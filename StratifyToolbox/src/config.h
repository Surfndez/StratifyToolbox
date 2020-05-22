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


#ifndef CONFIG_H_
#define CONFIG_H_

#include <mcu/arch.h>

#include "sl_config.h"
#include "board_config.h"

/*
 * openocd -f interface/stlink-v2-1.cfg -f target/stm32h7x_stlink.cfg
 * openocd -f StratifyToolbox/stlink-fixed.cfg -f StratifyToolbox/stm32h7x.cfg -c "program ./StratifyToolbox/build_boot_debug/StratifyToolbox.bin 0x08000000; reset run; exit;"
 *
 *
 * sl fs.copy:source=host@StratifyToolbox/build_debug/StratifyToolbox.bin,dest=device@/home/debug.bin
 * sl fs.verify:source=host@StratifyToolbox/build_debug/StratifyToolbox.bin,dest=device@/home/debug.bin
 *
 * sl fs.write:source=host@StratifyToolbox/build_debug/StratifyToolbox.bin,dest=device@/dev/ramdrive
 * sl fs.verify:source=host@StratifyToolbox/build_debug/StratifyToolbox.bin,dest=device@/dev/ramdrive
 * sl task.signal:id=1,signal=ALARM
 *
 * sl --writeOsRam && sl --execOsRam
 *
 */

#define SOS_BOARD_SYSTEM_CLOCK 480000000
#define SOS_BOARD_SYSTEM_MEMORY_SIZE (64*1024)

#define SOS_BOARD_USB_RX_BUFFER_SIZE 512
#define SOS_BOARD_STDIO_BUFFER_SIZE 512
#define SOS_BOARD_TMR 4 //TIM5 -- 32-bit timer

/*
 * 8MB External flash drive
 *
 * 2MB Flash OS
 * 2MB Data Assets
 * 3MB Applications Assets
 * 512KB RAM OS copy
 *
 */

#define SOS_BOARD_EXTERNAL_FLASH_SIZE (1024*1024UL*8)

#define SOS_BOARD_FLASH_OS_SIZE (2*1024*1024UL)
#define SOS_BOARD_FLASH_OS_ADDRESS (0x90000000)
#define SOS_BOARD_FLASH_OS_OFFSET (0)

#define SOS_BOARD_DATA_ASSETS_ADDRESS (SOS_BOARD_FLASH_OS_ADDRESS + SOS_BOARD_FLASH_OS_SIZE)
#define SOS_BOARD_DATA_ASSETS_OFFSET (SOS_BOARD_FLASH_OS_OFFSET + SOS_BOARD_FLASH_OS_SIZE)
#define SOS_BOARD_DATA_ASSETS_SIZE (2*1024*1024UL)

#define SOS_BOARD_APPLICATION_ASSETS_ADDRESS (SOS_BOARD_DATA_ASSETS_ADDRESS + SOS_BOARD_DATA_ASSETS_SIZE)
#define SOS_BOARD_APPLICATION_ASSETS_OFFSET (SOS_BOARD_DATA_ASSETS_OFFSET + SOS_BOARD_DATA_ASSETS_SIZE)
#define SOS_BOARD_APPLICATION_ASSETS_SIZE (3*1024*1024UL + 512*1024UL)

#define SOS_BOARD_RAM_OS_ADDRESS (0x24000000)
#define SOS_BOARD_RAM_OS_FLASH_ADDRESS (SOS_BOARD_APPLICATION_ASSETS_ADDRESS+SOS_BOARD_APPLICATION_ASSETS_SIZE)
#define SOS_BOARD_RAM_OS_FLASH_OFFSET (SOS_BOARD_APPLICATION_ASSETS_OFFSET+SOS_BOARD_APPLICATION_ASSETS_SIZE)
#define SOS_BOARD_RAM_OS_SIZE (512*1024UL)

#define SOS_BOARD_DRIVE0_SIZE (8*1024*1024UL)
#define SOS_BOARD_RAM_OS_ADDRESS (0x24000000)

//Total number of tasks (threads) for the entire system
#define SOS_BOARD_TASK_TOTAL 10
#define SOS_BOARD_EVENT_HANDLER board_event_handler
#define SOS_BOARD_TRACE_EVENT board_trace_event

#define STM32_ARCH_O_FLAGS STM32_CONFIG_FLAG_IS_HSE_ON
#define STM32_ARCH_CLOCK_PLLM 4
#define STM32_ARCH_CLOCK_PLLN 216
#define STM32_ARCH_CLOCK_PLLP 2
#define STM32_ARCH_CLOCK_PLLQ 9
#define STM32_ARCH_CLOCK_PLLR 2
#define STM32_ARCH_CLOCK_AHB_CLOCK_DIVIDER 1
#define STM32_ARCH_CLOCK_APB1_CLOCK_DIVIDER 4
#define STM32_ARCH_CLOCK_APB2_CLOCK_DIVIDER 2
#define STM32_ARCH_CLOCK_48_CLOCK_SELECTION 0
#define STM32_ARCH_CLOCK_VOLTAGE_SCALE 1
#define STM32_ARCH_CLOCK_FLASH_LATENCY 7

//--------------------------------------------Symbols-------------------------------------------------

/* By defining Ignore switches, functions can be omitted from the kernel
 * This means any applications that use these functions will fail
 * to install on the BSP.
 *
 * If you are building a custom board, ignoring symbols is a good
 * way to squeeze down the kernel to only what is necessary. However,
 * if you plan on allowing your users to install applications, they
 * might find it challenging when some functions are missing (the
 * applications will compile but fail to install).
 *
 * See [sos/symbols/defines.h](https://github.com/StratifyLabs/StratifyOS/blob/master/include/sos/symbols/defines.h)
 * for all available switches.
 *
 */

#define SYMBOLS_IGNORE_DCOMPLEX 1
#define SYMBOLS_IGNORE_POSIX_TRACE 1 //Always ignore -- deprecated
#define SYMBOLS_IGNORE_SG 1 //Stratify Graphics -- ignore if board will not support displays
#define SYMBOLS_IGNORE_SOCKET 1
#define SYMBOLS_IGNORE_LWIP 1

//#define SOS_BOARD_ARM_DSP_API_Q15 1
//#define SOS_BOARD_ARM_DSP_API_Q31 1
#define ARM_DSP_API_IGNORE_FILTER 1
#define ARM_DSP_API_IGNORE_MATRIX 0
#define ARM_DSP_API_IGNORE_CONTROLLER 1
#define ARM_DSP_API_IGNORE_DCT 1
#define SOS_BOARD_ARM_DSP_API_F32 1
#define SOS_BOARD_ARM_DSP_CONVERSION_API 1

//other ignore switches
#if 0
#define SYMBOLS_IGNORE_MATH_F 1
#define SYMBOLS_IGNORE_DOUBLE 1
#define SYMBOLS_IGNORE_STDIO_FILE 1
#define SYMBOLS_IGNORE_SIGNAL 1
#define SYMBOLS_IGNORE_PTHREAD_MUTEX 1
#define SYMBOLS_IGNORE_PTHREAD_COND 1
#define SYMBOLS_IGNORE_AIO 1
#define SYMBOLS_IGNORE_WCTYPE 1
#define SYMBOLS_IGNORE_STR 1
#define SYMBOLS_IGNORE_SEM 1
#define SYMBOLS_IGNORE_MQ 1
#endif





#endif /* CONFIG_H_ */
