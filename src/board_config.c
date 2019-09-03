/*

Copyright 2011-2018 Stratify Labs, Inc

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	 http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

#include <string.h>
#include <fcntl.h>
#include <sos/sos.h>
#include <mcu/debug.h>
#include <cortexm/task.h>
#include <sos/link/types.h>
#include <mcu/bootloader.h>
#include <mcu/pio.h>
#include <cortexm/mpu.h>
#include <mcu/tmr.h>

#include "board_config.h"
#include "config.h"
#include "link_config.h"
#include "stm32_bsp.h"
#include "display_device.h"
#include "st7789h2.h"

#define TRACE_COUNT 8
#define TRACE_FRAME_SIZE sizeof(link_trace_event_t)
#define TRACE_BUFFER_SIZE (sizeof(link_trace_event_t)*TRACE_COUNT)
static char trace_buffer[TRACE_FRAME_SIZE*TRACE_COUNT];
const ffifo_config_t board_trace_config = {
	.frame_count = TRACE_COUNT,
	.frame_size = sizeof(link_trace_event_t),
	.buffer = trace_buffer
};
ffifo_state_t board_trace_state;

typedef struct MCU_PACK {
	u16 signature;
	u32 size;
	u16 resd1;
	u16 resd2;
	u32 offset;
} bmp_header_t;

typedef struct MCU_PACK {
	u32 hdr_size;
	s32 width;
	s32 height;
	u16 planes;
	u16 bits_per_pixel;
} bmp_dib_t;

#if _IS_BOOT
int load_kernel_image();

const bootloader_board_config_t boot_board_config = {
	.sw_req_loc = 0x20002000, //needs to reside in RAM that is preserved through reset and available to both bootloader and OS
	.sw_req_value = 0x55AA55AA, //this can be any value
	.program_start_addr = 0x24000000, //RAM image starts here
	.hw_req.port = 0xff, .hw_req.pin = 0xff,
	.o_flags = 0,
	.link_transport_driver = 0,
	.id = 1,
};


static void * stack_ptr;
static void (*app_reset)();
#define SYSTICK_CTRL_TICKINT (1<<1)

static void handle_alarm(int signo){
	mcu_debug_printf("signal %d\n", signo);

	if( signo > 0 ){
		sleep(1); //wait one second to allow the sl command to complete
	}

	sos_link_transport_usb_close(&link_transport.handle);
	cortexm_svcall((cortexm_svcall_t)cortexm_set_privileged_mode, 0);



	//shutdown the system timer
	devfs_handle_t tmr_handle;
	tmr_handle.port = SOS_BOARD_TMR;
	mcu_tmr_close(&tmr_handle);

	u32 * start_of_program = (u32*)(boot_board_config.program_start_addr);
	stack_ptr = (void*)start_of_program[0];
	app_reset = (void (*)())( start_of_program[1] );

	devfs_handle_t uart_handle;
	uart_handle.port = 1;
	mcu_uart_close(&uart_handle);

	//turn off MPU, disable interrupts and SYSTICK
	cortexm_reset_mode();

	mcu_core_disable_cache();


	//assign the value of the MSP based on the RAM image, currently the PSP is in use so it can be a call
	cortexm_set_stack_ptr(stack_ptr);

	/*
	 * Stop using the PSP. This must be inlined. If a call was made,
	 * the return would cause problems because lr would be popped
	 * from the wrong stack.
	 */
	register u32 control;
	control = __get_CONTROL();
	control &= ~0x03;
	__set_CONTROL(control);

	/*
	 * This will start executing the OS that is currently in RAM.
	 *
	 */
	app_reset();

	//should never get here
	sos_led_root_error(0);
}

#endif

void write_row_svcall(void * args){
	CORTEXM_SVCALL_ENTER();
	LCD_IO_WriteDataBlock(args, 600);
}

void set_cursor_svcall(void * args){
	CORTEXM_SVCALL_ENTER();
	int j = (int)args;
	u8 parameter[4];

	/* Set Column address CASET */
	parameter[0] = 0;
	parameter[1] = 10;
	parameter[2] = 1;
	parameter[3] = 53;
	ST7789H2_WriteReg(ST7789H2_CASET, parameter, 4);

	/* Set Row address RASET */
	parameter[0] = 0;
	parameter[1] = 146-j;
	parameter[2] = 0;
	parameter[3] = 146;
	ST7789H2_WriteReg(ST7789H2_RASET, parameter, 4);

	ST7789H2_WriteReg(ST7789H2_WRITE_RAM, 0, 0);

}

void board_trace_event(void * event){
	link_trace_event_header_t * header = event;
	devfs_async_t async;
	const devfs_device_t * trace_dev = &(devfs_list[0]);

	//write the event to the fifo
	memset(&async, 0, sizeof(devfs_async_t));
	async.tid = task_get_current();
	async.buf = event;
	async.nbyte = header->size;
	async.flags = O_RDWR;
	trace_dev->driver.write(&(trace_dev->handle), &async);
}


void board_event_handler(int event, void * args){

	pio_attr_t attr;
	devfs_handle_t pio_handle;
	switch(event){
		case MCU_BOARD_CONFIG_EVENT_ROOT_TASK_INIT:
			//check for boot to OS request
			break;

		case MCU_BOARD_CONFIG_EVENT_ROOT_FATAL:
			//start the bootloader on a fatal event
			//mcu_core_invokebootloader(0, 0);
			if( args != 0 ){
				mcu_debug_log_fatal(MCU_DEBUG_SYS, "Fatal Error %s", (const char*)args);
			} else {
				mcu_debug_log_fatal(MCU_DEBUG_SYS, "Fatal Error unknown");
			}
			sos_led_root_error();

			while(1){}

			break;

		case MCU_BOARD_CONFIG_EVENT_ROOT_INITIALIZE_CLOCK:
			SystemClock_Config();

#if !_IS_BOOT


#endif


			//PE1 needs to be driven low for debugging to work
			attr.o_flags = PIO_FLAG_SET_OUTPUT;
			attr.o_pinmask = (1<<1);
			pio_handle.port = 4;
			pio_handle.config = 0;
			pio_handle.state = 0;

			mcu_pio_open(&pio_handle);
			mcu_pio_setattr(&pio_handle, &attr);
			mcu_pio_clrmask(&pio_handle, (void*)attr.o_pinmask);

			attr.o_flags = PIO_FLAG_SET_INPUT | PIO_FLAG_IS_FLOAT;
			attr.o_pinmask = (1<<1);
			pio_handle.port = 0;
			mcu_pio_open(&pio_handle);
			mcu_pio_setattr(&pio_handle, &attr);

			break;

		case MCU_BOARD_CONFIG_EVENT_ROOT_DEBUG_INITIALIZED:

			mcu_debug_printf("hello\n");
			ST7789H2_Init();



			break;

		case MCU_BOARD_CONFIG_EVENT_START_LINK:
			mcu_debug_log_info(MCU_DEBUG_USER0, "Start LED");

#if _IS_BOOT


			int fd = open("/home/icon.bmp", O_RDONLY);
			if( fd >= 0 ){
				mcu_debug_printf("opened %d\n", fd);

				bmp_header_t hdr;
				bmp_dib_t dib;

				if( read(fd, &hdr, sizeof(hdr) == sizeof(hdr)) ){
					mcu_debug_printf("read header\n");
					if( read(fd, &dib, sizeof(dib) == sizeof(dib)) ){

						lseek(fd, 138, SEEK_SET);
						mcu_debug_printf("height is %d\n", dib.height);
						mcu_debug_printf("signature is %d\n", hdr.signature);
						mcu_debug_printf("size is %d\n", hdr.size);
						mcu_debug_printf("offset is %d\n", hdr.offset);

						u16 row_buffer[300];

						for(u32 j = 0; j < 54; j++){
							mcu_debug_printf("write %d of %d\n", j, dib.height);
							read(fd, row_buffer, sizeof(row_buffer));
							cortexm_svcall(set_cursor_svcall, (void*)j);

							for(u32 i=0; i < 300; i++){
								row_buffer[i] = __REV16(row_buffer[i]);
							}
							cortexm_svcall(write_row_svcall, row_buffer);
						}


					} else {
						mcu_debug_printf("failed to read dib\n");
					}


				} else {
					mcu_debug_printf("failed to read hdr\n");

				}

				close(fd);
			} else {
				mcu_debug_printf("failed to open file\n");
			}

			//if( load_kernel_image() < 0 ){
			if( 1 ){
				sos_led_startup();
				mcu_debug_log_error(MCU_DEBUG_USER0, "failed to load kernel image");
				signal(SIGALRM, handle_alarm);
			} else {
				MCU_DEBUG_LINE_TRACE();
				handle_alarm(0);
			}
#else
			sos_led_startup();
			mcu_debug_log_info(MCU_DEBUG_USER0, "Booting from RAM");
#endif
			break;

		case MCU_BOARD_CONFIG_EVENT_START_FILESYSTEM:
			break;
	}
}

#if _IS_BOOT
typedef struct {
	u32 offset;
	const void * src;
	u32 size;
} copy_block_t;

void svcall_copy_block(void * args){
	CORTEXM_SVCALL_ENTER();
	copy_block_t * p = args;
	memcpy((void*)(0x24000000 + p->offset), p->src, p->size);
}

int load_kernel_image(){
	int fd;
	fd = open("/home/debug.bin", O_RDONLY);
	if( fd < 0 ){
		mcu_debug_log_warning(MCU_DEBUG_USER0, "debug image not found");
		return -1;
	}

	struct stat st;

	fstat(fd, &st);

	mcu_debug_log_warning(MCU_DEBUG_USER0, "debug image is %ld bytes", st.st_size);

	//need to disable MPU
	MCU_DEBUG_LINE_TRACE();
	char buffer[256];
	int result = 0;
	copy_block_t args;

	args.offset = 0;
	args.size = 256;
	args.src = buffer;
	do {
		result = read(fd, buffer, 256);
		if( result > 0 ){
			args.size = result;
			cortexm_svcall(svcall_copy_block, &args);
			args.offset += result;
		}
		//this is causing a memory fault (but MPU is disabled??)
	} while( result > 0 );
	MCU_DEBUG_LINE_TRACE();

	close(fd);



	return 0;
}

void exec_bootloader(void * args){
	//write SW location with key and then reset

	cortexm_reset(0);
}

void boot_event(int event, void * args){
	mcu_board_execute_event_handler(event, args);
}


const bootloader_api_t mcu_core_bootloader_api = {
	.code_size = (u32)&_etext,
	.exec = 0,
	.usbd_control_root_init = 0,
	.event = boot_event
};
#endif
