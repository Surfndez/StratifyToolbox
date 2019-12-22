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

#include "board_config.h"
#include "config.h"
#include "link_config.h"
#include "devfs/st7789h2.h"
#include "mcu/stm32_bsp.h"
#include "kernel/kernel_loader.h"
#include "kernel/kernel_service.h"

#define TRACE_COUNT 8
#define TRACE_FRAME_SIZE sizeof(link_trace_event_t)
#define TRACE_BUFFER_SIZE (sizeof(link_trace_event_t)*TRACE_COUNT)
static char trace_buffer[TRACE_FRAME_SIZE*TRACE_COUNT];
const ffifo_config_t board_trace_config = {
	TRACE_COUNT,
	sizeof(link_trace_event_t),
	trace_buffer
};
ffifo_state_t board_trace_state;


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

#if 0
			//PE1 needs to be driven low for debugging to work
			attr.o_flags = PIO_FLAG_SET_OUTPUT;
			attr.o_pinmask = (1<<1);
			pio_handle.port = 4;
			pio_handle.config = 0;
			pio_handle.state = 0;

			mcu_pio_open(&pio_handle);
			mcu_pio_setattr(&pio_handle, &attr);
			mcu_pio_clrmask(&pio_handle, (void*)attr.o_pinmask);
#endif

		case MCU_BOARD_CONFIG_EVENT_ROOT_DEBUG_INITIALIZED:
			ST7789H2_Init();
			break;

		case MCU_BOARD_CONFIG_EVENT_START_LINK:
			mcu_debug_log_info(MCU_DEBUG_USER0, "Start LED");

#if _IS_BOOT
			kernel_loader_startup();
#else
			sos_led_startup();
			kernel_service_init();
			mcu_debug_log_info(MCU_DEBUG_USER0, "Booting from RAM");
#endif
			break;

		case MCU_BOARD_CONFIG_EVENT_START_FILESYSTEM:
			break;
	}
}
