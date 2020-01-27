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
#include <sos/crypt_api.h>
#include <sos/dev/drive.h>

#include "kernel_loader.h"
#include "config.h"
#include "link_config.h"
#include "mcu/stm32_bsp.h"
#include "devfs/display_device.h"
#include "devfs/st7789h2.h"

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

#define KERNEL_IMAGE_SIZE (512*1024)
#define KERNEL_HASH_SIZE (32)

static void execute_ram_image(int signo);
static void erase_flash_image(int signo);
static void svcall_is_bootloader_requested(void * args);
static int load_kernel_image(u32 offset, u32 is_load);
static void write_row_svcall(void * args);
static void set_cursor_svcall(void * args);
static void clear_display_svcall(void * args);

const bootloader_board_config_t boot_board_config = {
	.sw_req_loc = 0x20002000, //needs to reside in RAM that is preserved through reset and available to both bootloader and OS
	.sw_req_value = 0x55AA55AA, //this can be any value
	.program_start_addr = 0x24000000, //RAM image starts here
	.hw_req.port = 0xff, .hw_req.pin = 0xff,
	.o_flags = 0,
	.link_transport_driver = 0,
	.id = __HARDWARE_ID,
};


int kernel_loader_startup(){
	int fd = open("/home/icon.bmp", O_RDONLY);
	if( fd >= 0 ){
		bmp_header_t hdr;
		bmp_dib_t dib;

		struct timespec start, stop;

		clock_gettime(CLOCK_REALTIME, &start);
		if( read(fd, &hdr, sizeof(hdr) == sizeof(hdr)) ){
			if( read(fd, &dib, sizeof(dib) == sizeof(dib)) ){
				lseek(fd, 138, SEEK_SET);
				u16 row_buffer[300];
				for(u32 j = 0; j < 54; j++){
					read(fd, row_buffer, sizeof(row_buffer));
					cortexm_svcall(set_cursor_svcall, (void*)j);
					cortexm_svcall(write_row_svcall, row_buffer);
				}
			} else {
				mcu_debug_printf("failed to read dib\n");
			}
		} else {
			mcu_debug_printf("failed to read hdr\n");

		}
		close(fd);
		clock_gettime(CLOCK_REALTIME, &stop);

		mcu_debug_log_info(
					MCU_DEBUG_USER0,
					"refresh time %ld",
					(stop.tv_nsec - start.tv_nsec)/1000
					);

#if 0
		clock_gettime(CLOCK_REALTIME, &start);
		cortexm_svcall(clear_display_svcall, 0);
		clock_gettime(CLOCK_REALTIME, &stop);

		mcu_debug_log_info(
					MCU_DEBUG_USER0,
					"clear time %ld",
					(stop.tv_nsec - start.tv_nsec)/1000
					);
#endif

	} else {
		mcu_debug_log_warning(
					MCU_DEBUG_USER0,
					"failed to open splash image"
					);
	}

	int is_bootloader_requested;
	cortexm_svcall(
				svcall_is_bootloader_requested,
				&is_bootloader_requested
				);

	if( is_bootloader_requested == 0 &&
		 ( (load_kernel_image(0, 1) == 0)
			//|| (load_kernel_image(KERNEL_IMAGE_SIZE, 1) == 0)
			) ){
		execute_ram_image(0);
	}


	mcu_debug_log_info(MCU_DEBUG_USER0, "bootloader running");
	signal(SIGALRM, erase_flash_image);
	signal(SIGCONT, execute_ram_image);

	for(u32 i=0; i < 4; i++){
		cortexm_svcall(sos_led_svcall_enable, 0);
		usleep(50*1000);
		cortexm_svcall(sos_led_svcall_disable, 0);
		usleep(50*1000);
	}
	return 0;
}

static void * stack_ptr;
static void (*app_reset)();
#define SYSTICK_CTRL_TICKINT (1<<1)

void erase_flash_image(int signo){
	int result;
	mcu_debug_log_info(MCU_DEBUG_USER0, "signal %d", signo);

	//execute an erase on /dev/drive1 so that a new image can be installed
	int fd = open("/dev/drive1", O_RDWR);
	if( fd < 0 ){
		mcu_debug_log_error(MCU_DEBUG_USER0, "failed to open drive1");
		return;
	}

	drive_info_t info;
	ioctl(fd, I_DRIVE_GETINFO, &info);

	u32 offset = 0;
	drive_attr_t attributes;
	u32 address = offset;
	do {

		if( (address % (128*1024)) == 0 ){
			cortexm_svcall(sos_led_svcall_enable, 0);
		}

		mcu_debug_printf("Erase block at address %ld\n", address);

		while( ioctl(fd, I_DRIVE_ISBUSY) > 0 ){
			usleep(info.erase_block_time);
		}

		attributes.o_flags = DRIVE_FLAG_ERASE_BLOCKS;
		attributes.start = address;
		attributes.end = info.num_write_blocks;
		if( (result = ioctl(fd, I_DRIVE_SETATTR, &attributes)) < 0 ){
			mcu_debug_log_info(
						MCU_DEBUG_USER0,
						"failed to erase drive (%d, %d)",
						result, errno);
			break;
		}
		address += result;
		cortexm_svcall(sos_led_svcall_disable, 0);

	} while(address < info.num_write_blocks );

	while( ioctl(fd, I_DRIVE_ISBUSY) > 0 ){
		usleep(info.erase_block_time);
	}


	u8 read_buffer[32];
	u32 not_blank_count = 0;
	address = offset;
	while( (address < (offset + KERNEL_IMAGE_SIZE)) &&
			 ((result = read(fd, read_buffer, 32)) > 0)
			 ){
		for(u32 i=0; i < 32; i++){
			if( read_buffer[i] != 0xff ){
				not_blank_count++;
			}
		}
		if( result > 0 ){
			address += result;
		}
	}

	if( not_blank_count > 0 ){
		mcu_debug_printf("Not blank: %ld\n", not_blank_count);
	}

	close(fd);

	mcu_debug_log_info(
				MCU_DEBUG_USER0,
				"erase complete"
				);


}

void execute_ram_image(int signo){
	MCU_UNUSED_ARGUMENT(signo);

	if( signo == SIGCONT ){
		mcu_debug_log_info(MCU_DEBUG_USER0, "RAM exec requested");
		sleep(1);
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

	mcu_core_clean_data_cache();

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
	LCD_IO_WriteDataBlockRgb(args, 600);
}

void set_cursor_svcall(void * args){
	CORTEXM_SVCALL_ENTER();
	int j = (int)args;
	ST7789H2_SetWindow(10, 146-j, 300, 146-j+1);
	ST7789H2_WriteReg(ST7789H2_WRITE_RAM, 0, 0);
}

void clear_display_svcall(void * args){
	CORTEXM_SVCALL_ENTER();
	for(u32 i=0; i < DISPLAY_HEIGHT; i++){
		ST7789H2_DrawHLine(0, 0, i, ST7789H2_LCD_PIXEL_WIDTH);
	}
}

#if _IS_BOOT
typedef struct {
	u32 offset;
	const void * src;
	u32 size;
} copy_block_t;

void copy_block(void * args){
	copy_block_t * p = args;
	memcpy((void*)(0x24000000 + p->offset), p->src, p->size);
}

int load_kernel_image(u32 offset, u32 is_load){
	int image_fd;
	void * hash_context;
	u8 hash_digest[256/8];
	u8 check_hash_digest[256/8];
	const crypt_hash_api_t * hash_api = kernel_request_api(CRYPT_SHA256_API_REQUEST);
	if( hash_api == 0 ){
		mcu_debug_log_error(MCU_DEBUG_USER0, "hash API is missing");
		return -1;
	}

	if( hash_api->init(&hash_context) < 0 ){
		mcu_debug_log_error(MCU_DEBUG_USER0, "failed to init hash context");
		return -1;
	}

	image_fd = open("/dev/drive1", O_RDONLY);
	if( image_fd < 0 ){
		mcu_debug_log_warning(MCU_DEBUG_USER0, "debug image not found");
		hash_api->deinit(&hash_context);
		return -1;
	}

	//need to disable MPU
	u8 buffer[256];
	int result = 0;
	copy_block_t args;

	const u32 max_image_size =
			KERNEL_IMAGE_SIZE - KERNEL_HASH_SIZE;
	int page_size;
	args.offset = 0;
	args.size = 256;
	args.src = buffer;
	hash_api->start(hash_context);

	lseek(image_fd, offset, SEEK_SET);
	do {
		if( max_image_size - args.offset > 256 ){
			page_size = 256;
		} else {
			page_size = max_image_size - args.offset;
		}
		result = read(image_fd, buffer, page_size);

		if( result > 0 ){
			hash_api->update(
						hash_context,
						buffer,
						result
						);
			args.size = result;
			if( is_load != 0 ){
				cortexm_svcall(copy_block, &args);
			}
			args.offset += result;
		}

	} while(
			  (result > 0) &&
			  (args.offset < max_image_size)
			  );

	hash_api->finish(
				hash_context,
				hash_digest,
				256/8
				);

	hash_api->deinit(
				&hash_context
				);

	//make sure the image is valid by checking the hash
	result = read(
				image_fd,
				check_hash_digest,
				sizeof(check_hash_digest)
				);

	close(image_fd);

	if( result == sizeof(check_hash_digest) ){
		if( memcmp(
				 hash_digest,
				 check_hash_digest,
				 KERNEL_HASH_SIZE
				 ) != 0 ){
			mcu_debug_log_info(
						MCU_DEBUG_USER0,
						"hash is bad from %ld bytes",
						args.offset);
			for(u32 i=0; i < KERNEL_HASH_SIZE; i++){
				mcu_debug_printf("%d = 0x%X == 0x%X\n", i, hash_digest[i], check_hash_digest[i]);
			}
			return -1;
		}
	}

	return 0;
}


void exec_bootloader(void * args){
	//write SW location with key and then reset
	//this is execute in privileged mode
	u32 * bootloader_request_address = (u32*)0x30020000;
	*bootloader_request_address = 0xaabbccdd;
	mcu_core_clean_data_cache();

	cortexm_reset(0);
}

void svcall_is_bootloader_requested(void * args){
	int * is_requested = args;
	u32 * bootloader_request_address = (u32*)0x30020000;

	if( *bootloader_request_address == 0xaabbccdd ){
		*bootloader_request_address = 0;
		mcu_core_clean_data_cache();
		*is_requested = 1;
	} else {
		*is_requested = 0;
	}
}

void boot_event(int event, void * args){
	mcu_board_execute_event_handler(event, args);
}


const bootloader_api_t mcu_core_bootloader_api = {
	.code_size = (u32)&_etext,
	.exec = exec_bootloader,
	.usbd_control_root_init = 0,
	.event = boot_event
};
#endif
