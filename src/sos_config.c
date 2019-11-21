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


#include <sys/lock.h>
#include <fcntl.h>
#include <errno.h>
#include <mcu/mcu.h>
#include <mcu/debug.h>
#include <mcu/periph.h>
#include <device/sys.h>
#include <device/null.h>
#include <device/zero.h>
#include <device/random.h>
#include <device/uartfifo.h>
#include <device/usbfifo.h>
#include <device/fifo.h>
#include <device/sys.h>
#include <sos/link.h>
#include <sos/fs/sysfs.h>
#include <sos/fs/appfs.h>
#include <sos/fs/devfs.h>
#include <sos/fs/sffs.h>
#include <mcu/appfs.h>
#include <sos/sos.h>
#include <device/drive_cfi.h>

#include "ram_drive.h"

#include "config.h"
#include "link_config.h"
#include "wifi_api.h"

//--------------------------------------------Socket API-------------------------------------------------

#if !_IS_BOOT
#define WIFI_PACKET_BUFFER_SIZE 64
u8 wifi_packet_buffer[WIFI_PACKET_BUFFER_SIZE];
wifi_api_state_t wifi_api_state;

const wifi_api_config_t wifi_api_config = {
	.device_config = {
		.devfs = &(sysfs_list[1]),
		.name = "wifi_phy",
		.state = &wifi_api_state.device_state
	},
	.hw_addr[0] = 0,
	.hw_addr[1] = 0,
	.hw_addr[2] = 0,
	.hw_addr[3] = 0,
	.hw_addr[4] = 0,
	.hw_addr[5] = 0,
	.mtu = 1500,
	.host_name = "StratifyToolbox",
	.network_interface_list = 0,
	.thread_stack_size = 4096,
	.network_interface_count = 0,
	.max_packet_size = WIFI_PACKET_BUFFER_SIZE,
	.packet_buffer = wifi_packet_buffer
};


WIFI_DECLARE_SOCKET_API(wifi, &wifi_api_config, &wifi_api_state);

#endif
//--------------------------------------------Stratify OS Configuration-------------------------------------------------
const sos_board_config_t sos_board_config = {
	.clk_usecond_tmr = SOS_BOARD_TMR, //TIM2 -- 32 bit timer
	.task_total = SOS_BOARD_TASK_TOTAL,
	.stdin_dev = "/dev/stdio-in" ,
	.stdout_dev = "/dev/stdio-out",
	.stderr_dev = "/dev/stdio-out",
	.o_sys_flags = SYS_FLAG_IS_STDIO_FIFO | SYS_FLAG_IS_TRACE | SYS_FLAG_IS_ACTIVE_ON_IDLE,
	.sys_name = SL_CONFIG_NAME,
	.sys_version = SL_CONFIG_VERSION_STRING,
	.sys_id = SL_CONFIG_DOCUMENT_ID,
	.team_id = "SL_CONFIG_TEAM_ID",
	.sys_memory_size = SOS_BOARD_SYSTEM_MEMORY_SIZE,
	.start = sos_default_thread,
	.start_args = &link_transport,
	.start_stack_size = 8192,
	#if !_IS_BOOT
	.socket_api = &wifi_api,
	#else
	.socket_api = 0,
	#endif
	.request = 0,
	.git_hash = SOS_GIT_HASH,
	.trace_dev = "/dev/trace",
	.trace_event = SOS_BOARD_TRACE_EVENT
};

//This declares the task tables required by Stratify OS for applications and threads
SOS_DECLARE_TASK_TABLE(SOS_BOARD_TASK_TOTAL);


//--------------------------------------------Root Filesystem---------------------------------------------------

/*
 * This is the root filesystem that determines what is mounted at /.
 *
 * The default is /app (for installing and running applciations in RAM and flash) and /dev which
 * provides the device tree defined above.
 *
 * Additional filesystems (such as FatFs) can be added if the hardware and drivers
 * are provided by the board.
 *
 */

#if 1
#define INTERNAL_RAM_PAGE_COUNT0 288
#define INTERNAL_RAM_PAGE_COUNT1 64
#define TCM_RAM_PAGE_COUNT 64
#define APPFS_RAM_PAGES (TCM_RAM_PAGE_COUNT + INTERNAL_RAM_PAGE_COUNT0 + INTERNAL_RAM_PAGE_COUNT1)

/*
 * Memory
 *
 * ITCM 0x00000000 to 0x0000FFFF 64KB (Not Used)
 * DTCM 0x20000000 to 0x2001FFFF 128KB (first half is SYSMEM)
 *
 * AXI SRAM 0x24000000 to 0x2407FFFF 512KB (Code + Data Region for OS) on AXIM
 * SRAM1 0x30000000 to 0x3001FFFF 128KB (application) D1 to D2 AHB to AXIM
 * SRAM2 0x30020000 to 0x3003FFFF 128KB D1 to D2 AHB to AXIM
 * SRAM3 0x30040000 to 0x30047FFF 32KB D1 to D2 AHB to AXIM
 * SRAM4 0x38000000 to 0x3800FFFF 64KB D1 to D3 AHB to AXIM
 * BACKUP SRAM 0x38800000 to 0x3800FFF 4KB
 *
 * SRAM1,2,3 are contiguous
 *
 *
 */

u32 ram_usage_table[APPFS_RAM_USAGE_WORDS(APPFS_RAM_PAGES)] MCU_SYS_MEM;
const devfs_device_t flash0 =
		DEVFS_DEVICE(
			"flash0",
			mcu_flash,
			0,
			0,
			0,
			0666,
			SYSFS_ROOT,
			S_IFBLK
			);

const appfs_mem_config_t appfs_mem_config = {
	.usage_size = sizeof(ram_usage_table),
	.usage = ram_usage_table,
	.system_ram_page = (u32)APPFS_RAM_PAGES, //system RAM is not located in the APPFS memory sections
	.flash_driver = &flash0,
	.section_count = 3,
	.sections = {
		{ .o_flags = MEM_FLAG_IS_RAM, .page_count = INTERNAL_RAM_PAGE_COUNT0, .page_size = MCU_RAM_PAGE_SIZE, .address = 0x30000000 },
		{ .o_flags = MEM_FLAG_IS_RAM, .page_count = INTERNAL_RAM_PAGE_COUNT1, .page_size = MCU_RAM_PAGE_SIZE, .address = 0x38000000 },
		{ .o_flags = MEM_FLAG_IS_RAM | MEM_FLAG_IS_TIGHTLY_COUPLED, .page_count = TCM_RAM_PAGE_COUNT, .page_size = MCU_RAM_PAGE_SIZE, .address = 0x20000000+64*1024 },
	}
};

const devfs_device_t mem0 =
		DEVFS_DEVICE(
			"mem0",
			appfs_mem,
			0,
			&appfs_mem_config,
			0,
			0666,
			SYSFS_ROOT,
			S_IFBLK
			);
#endif

sffs_state_t sffs_state;
const sffs_config_t sffs_configuration = {
	.drive = {
		.devfs = &(sysfs_list[1]),
		.name = "drive0",
		.state = (sysfs_shared_state_t*)&sffs_state
	}
};


#if !_IS_BOOT
#include <sos/fs/fatfs.h>

fatfs_state_t fatfs_state;
sysfs_file_t fatfs_open_file; // Cannot be in MCU_SYS_MEM because it is accessed in unpriv mode
const fatfs_config_t fatfs_configuration = {
	.drive = {
		.devfs = &(sysfs_list[1]),
		.name = "drive2",
		.state = &fatfs_state.drive
	},
	.wait_busy_microseconds = 500,
	.wait_busy_timeout_count = 5,
	.vol_id = 0
};
#endif


const sysfs_t sysfs_list[] = {
	APPFS_MOUNT("/app", &mem0, 0777, SYSFS_ROOT), //the folder for ram/flash applications
	DEVFS_MOUNT("/dev", devfs_list, 0555, SYSFS_ROOT), //the list of devices
	SFFS_MOUNT("/home", &sffs_configuration, 0777, SYSFS_ROOT), //stratify flash filesystem
	#if !_IS_BOOT
	FATFS_MOUNT("/card", &fatfs_configuration, 0777, SYSFS_ROOT),
	#endif
	SYSFS_MOUNT("/", sysfs_list, 0666, SYSFS_ROOT), //the root filesystem (must be last)
	SYSFS_TERMINATOR
};


