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
#include <mcu/hash.h>
#include <sos/sos.h>
#include <device/drive_cfi.h>
#include <device/drive_ram.h>
#include <device/drive_sdio.h>

#include "display_device.h"

#include "config.h"
#include "link_config.h"

//--------------------------------------------Device Filesystem-------------------------------------------------


/*
 * Defaults configurations
 *
 * This provides the default pin assignments and settings for peripherals. If
 * the defaults are not provided, the application must specify them.
 *
 * Defaults should be added for peripherals that are dedicated for use on the
 * board. For example, if a UART has an external connection and label on the
 * board, the BSP should provide the default configuration.
 *
 *
 *
 */

//PA2 and PA3 is USART2 (uart1)
UARTFIFO_DECLARE_CONFIG_STATE(uart0_fifo, 1024, 64,
										UART_FLAG_SET_LINE_CODING_DEFAULT, 8, 115200,
										0, 2,
										0, 3,
										0xff, 0xff,
										0xff, 0xff);

FIFO_DECLARE_CONFIG_STATE(stdio_in, SOS_BOARD_STDIO_BUFFER_SIZE);
FIFO_DECLARE_CONFIG_STATE(stdio_out, SOS_BOARD_STDIO_BUFFER_SIZE);

//PB8 and PB9 are I2C1_SCL and I2C1_SDA (i2c1)
const i2c_config_t i2c0_config = {
	.attr = {
		.o_flags = I2C_FLAG_SET_MASTER,
		.pin_assignment = {
			.sda = {1, 8},
			.scl = {1, 9}
		},
		.freq = 100000,
	}
};

//PB10 and PB11 are I2C2_SCL and I2C2_SDA (i2c1)
const i2c_config_t i2c1_config = {
	.attr = {
		.o_flags = I2C_FLAG_SET_MASTER,
		.pin_assignment = {
			.sda = {1, 11},
			.scl = {1, 10}
		},
		.freq = 100000,
	}
};

//Internal I2C PA8, PC9 -> I2C3_SCL, I2C3_SDA (i2c2)
const i2c_config_t i2c2_config = {
	.attr = {
		.o_flags = I2C_FLAG_SET_MASTER,
		.pin_assignment = {
			.sda = {2, 9}, //PC9
			.scl = {0, 8} //PA8
		},
		.freq = 100000,
	}
};

//PC10, PC11, PC12 are SPI3_SCK, SPI3_MISO, SPI3_MOSI (spi2)
const stm32_spi_dma_config_t spi2_dma_config = {
	.spi_config = {
		.attr = {
			.o_flags = SPI_FLAG_SET_MASTER |
			SPI_FLAG_IS_FORMAT_SPI |
			SPI_FLAG_IS_MODE0,
			.pin_assignment = {
				.cs = {0xff, 0xff},
				.sck = {2, 10},
				.miso = {2, 11},
				.mosi = {2, 12},
			},
			.width = 8
		}
	},
	.dma_config = {
		.tx = {
			.dma_number = STM32_DMA1,
			.stream_number = 3,
			.channel_number = 62, //DMA_REQUEST_SPI3_TX
			.priority = STM32_DMA_PRIORITY_LOW,
			.o_flags = STM32_DMA_FLAG_IS_NORMAL |
			STM32_DMA_FLAG_IS_FIFO |
			STM32_DMA_FLAG_IS_MEMORY_TO_PERIPH |
			STM32_DMA_FLAG_IS_MEMORY_BYTE |
			STM32_DMA_FLAG_IS_PERIPH_BYTE
		},
		.rx = {
			.dma_number = STM32_DMA1,
			.stream_number = 2,
			.channel_number = 61, //DMA_REQUEST_SPI3_RX
			.priority = STM32_DMA_PRIORITY_LOW,
			.o_flags = STM32_DMA_FLAG_IS_NORMAL |
			STM32_DMA_FLAG_IS_FIFO |
			STM32_DMA_FLAG_IS_PERIPH_TO_MEMORY |
			STM32_DMA_FLAG_IS_MEMORY_BYTE |
			STM32_DMA_FLAG_IS_PERIPH_BYTE
		}
	}
};

//PE11, PE12, PE13, PE14 are SPI4_NSS, SPI4_SCK, SPI4_MISO, SPI4_MOSI (spi3)
const stm32_spi_dma_config_t spi3_dma_config = {
	.spi_config = {
		.attr = {
			.o_flags = SPI_FLAG_SET_MASTER |
			SPI_FLAG_IS_FORMAT_SPI |
			SPI_FLAG_IS_MODE0,
			.pin_assignment = {
				.cs = {4, 11},
				.sck = {4, 12},
				.miso = {4, 13},
				.mosi = {4, 14},
			},
			.width = 8
		}
	},
	.dma_config = {
		.tx = {
			.dma_number = STM32_DMA1,
			.stream_number = 1,
			.channel_number = 84, //DMA_REQUEST_SPI4_TX,
			.priority = STM32_DMA_PRIORITY_LOW,
			.o_flags = STM32_DMA_FLAG_IS_NORMAL |
			STM32_DMA_FLAG_IS_FIFO |
			STM32_DMA_FLAG_IS_MEMORY_TO_PERIPH |
			STM32_DMA_FLAG_IS_MEMORY_BYTE |
			STM32_DMA_FLAG_IS_PERIPH_BYTE
		},
		.rx = {
			.dma_number = STM32_DMA1,
			.stream_number = 1,
			.channel_number = 83, //DMA_REQUEST_SPI4_RX,
			.priority = STM32_DMA_PRIORITY_LOW,
			.o_flags = STM32_DMA_FLAG_IS_NORMAL |
			STM32_DMA_FLAG_IS_FIFO |
			STM32_DMA_FLAG_IS_PERIPH_TO_MEMORY |
			STM32_DMA_FLAG_IS_MEMORY_BYTE |
			STM32_DMA_FLAG_IS_PERIPH_BYTE
		}
	}
};

const stm32_qspi_dma_config_t qspi_dma_config = {
	.qspi_config = {
		.attr = {
			.o_flags = QSPI_FLAG_SET_MASTER,
			.freq = 40000000UL,
			.pin_assignment = {
				.data[0] = {3,11}, //PD11
				.data[1] = {3,12}, //PD12
				.data[2] = {4,2}, //PE2
				.data[3] = {3,13}, //PD13
				.sck = {1,2}, //PB2
				.cs = {1,6} //PB6
			}
		}
	},
	.dma_config = {
		.tx = {
			.dma_number = STM32_DMA2,
			.stream_number = 7,
			.channel_number = 3,
			.priority = STM32_DMA_PRIORITY_LOW,
			.o_flags = STM32_DMA_FLAG_IS_NORMAL |
			STM32_DMA_FLAG_IS_FIFO |
			STM32_DMA_FLAG_IS_MEMORY_TO_PERIPH |
			STM32_DMA_FLAG_IS_MEMORY_BYTE |
			STM32_DMA_FLAG_IS_PERIPH_BYTE
		},
		.rx = {
			.dma_number = STM32_DMA2,
			.stream_number = 7,
			.channel_number = 3,
			.priority = STM32_DMA_PRIORITY_LOW,
			.o_flags = STM32_DMA_FLAG_IS_NORMAL |
			STM32_DMA_FLAG_IS_FIFO |
			STM32_DMA_FLAG_IS_PERIPH_TO_MEMORY |
			STM32_DMA_FLAG_IS_MEMORY_BYTE |
			STM32_DMA_FLAG_IS_PERIPH_BYTE
		}
	}
};

const devfs_device_t qspi_drive_device =
		DEVFS_DEVICE(
			"qspi",
			mcu_qspi_dma,
			0,
			&qspi_dma_config,
			0,
			0666,
			SYSFS_ROOT,
			S_IFCHR
			);


#define DRIVE1_SIZE (512*1024UL)
#define DRIVE0_SIZE (8*1024*1024UL - DRIVE1_SIZE)
const drive_cfi_config_t drive0_cfi_config = {
	.serial_device = &qspi_drive_device,
	.info = {
		.addressable_size = 1,
		.write_block_size = 1, //smallest available write size
		.num_write_blocks = DRIVE0_SIZE,  //7MB + 512KB -- last 512KB is OS copy
		.erase_block_size = 4096, //smallest eraseable block
		.erase_block_time = 30000UL, //45ms typical
		.erase_sector_size = 64*1024, //a larger eraseable block
		.erase_sector_time = 500000UL, //700ms typical
		.erase_device_time = 140000000UL, //140s typical
		.bitrate = 66000000UL,
		.partition_start = 0
	},
	.opcode = {
		.write_enable = 0x06,
		.page_program = 0x02,
		.block_erase = 0x20,
		.sector_erase = 0xD8,
		.device_erase = 0xff, //must use block erase
		.fast_read = 0xEB,
		.power_up = 0xAB,
		.power_down = 0xB9,
		.enable_reset = 0x66,
		.reset = 0x99,
		.protect = 0x04, //NA --use write disable
		.unprotect = 0x04, //NA --use write disable
		.read_busy_status = 0x05, //busy bit is bit 0 of status register 1
		.busy_status_mask = 0x01,
		.enter_qpi_mode = 0x38,
		.enter_4byte_address_mode = 0xff, //NA
		.write_status = 0xff, //NA -- don't change initial status
		.initial_status_value = 0x00, //NA
		.page_program_size = 256,
		.read_dummy_cycles = 4,
		.write_dummy_cycles = 0
	},
	.cs = { 0xff, 0xff },
	.qspi_flags =
	QSPI_FLAG_IS_OPCODE_QUAD |
	QSPI_FLAG_IS_DATA_QUAD |
	QSPI_FLAG_IS_ADDRESS_QUAD |
	QSPI_FLAG_IS_ADDRESS_24_BITS
};

drive_cfi_state_t drive0_cfi_state MCU_SYS_MEM;

const drive_cfi_config_t drive1_cfi_config = {
	.serial_device = &qspi_drive_device,
	.info = {
		.addressable_size = 1,
		.write_block_size = 1, //smallest available write size
		.num_write_blocks = DRIVE1_SIZE,  //OS copy
		.erase_block_size = 4096, //smallest eraseable block
		.erase_block_time = 30000UL, //45ms typical
		.erase_device_time = 140000000UL, //140s typical
		.erase_sector_size = 64*1024, //a larger eraseable block
		.erase_sector_time = 500000UL, //700ms typical
		.bitrate = 66000000UL,
		.partition_start = DRIVE0_SIZE
	},
	.opcode = {
		.write_enable = 0x06,
		.page_program = 0x02,
		.block_erase = 0x20,
		.sector_erase = 0xD8,
		.device_erase = 0xff, //must use sector erase
		.fast_read = 0xEB,
		.power_up = 0xAB,
		.power_down = 0xB9,
		.enable_reset = 0x66,
		.reset = 0x99,
		.protect = 0x04, //NA --use write disable
		.unprotect = 0x04, //NA --use write disable
		.read_busy_status = 0x05, //busy bit is bit 0 of status register 1
		.busy_status_mask = 0x01,
		.enter_qpi_mode = 0x38,
		.enter_4byte_address_mode = 0xff, //NA
		.write_status = 0xff, //NA -- don't change initial status
		.initial_status_value = 0x00, //NA
		.page_program_size = 256,
		.read_dummy_cycles = 4,
		.write_dummy_cycles = 0
	},
	.cs = { 0xff, 0xff },
	.qspi_flags =
	QSPI_FLAG_IS_OPCODE_QUAD |
	QSPI_FLAG_IS_DATA_QUAD |
	QSPI_FLAG_IS_ADDRESS_QUAD |
	QSPI_FLAG_IS_ADDRESS_24_BITS
};
drive_cfi_state_t drive1_cfi_state MCU_SYS_MEM;

#if _IS_BOOT

const drive_ram_config_t drive_ram_config = {
	.memory = (void*)0x24000000,
	.size = 512*1024UL
};

#define USB_FIFO_BUFFER_SIZE (LINK2_MAX_PACKET_SIZE)
static char usb_fifo_buffer[USB_FIFO_BUFFER_SIZE] MCU_SYS_MEM;
static char usb_read_buffer[SOS_LINK_TRANSPORT_USB_BULK_ENDPOINT_SIZE] MCU_SYS_MEM;
const usbfifo_config_t link2_transport_usb_fifo_cfg = {
	.endpoint = SOS_LINK_TRANSPORT_USB_BULK_ENDPOINT,
	.endpoint_size = SOS_LINK_TRANSPORT_USB_BULK_ENDPOINT_SIZE,
	.read_buffer = usb_read_buffer,
	.fifo = {
		.buffer = usb_fifo_buffer,
		.size = USB_FIFO_BUFFER_SIZE
	}
};
#define LINK_CONFIG &link2_transport_usb_fifo_cfg
#else
#define LINK_CONFIG &sos_link_transport_usb_fifo_cfg

drive_sdio_state_t sdio_state;
const drive_sdio_config_t sdio_configuration = {
	.sdio = {
		.attr = {
			.o_flags = SDIO_FLAG_SET_INTERFACE |
			SDIO_FLAG_IS_BUS_WIDTH_4 |
			SDIO_FLAG_IS_CLOCK_RISING |
			SDIO_FLAG_IS_HARDWARE_FLOW_CONTROL_ENABLED |
			0,
			.freq = 25000000UL,
			.pin_assignment = {
				.clock = {2, 1}, //PC1
				.command = {0, 0}, //PA0
				.data[0] = {1, 14}, //PB14
				.data[1] = {1, 15}, //PB15
				.data[2] = {1, 3}, //PB3
				.data[3] = {1, 4} //PB4
			}
		}
	}
};
#endif


/* This is the list of devices that will show up in the /dev folder.
 */
const devfs_device_t devfs_list[] = {
	//System devices
	#if !_IS_BOOT
	DEVFS_DEVICE("trace", ffifo, 0, &board_trace_config, &board_trace_state, 0666, SYSFS_ROOT, S_IFCHR),
	DEVFS_DEVICE("null", null, 0, 0, 0, 0666, SYSFS_ROOT, S_IFCHR),
	DEVFS_DEVICE("zero", zero, 0, 0, 0, 0666, SYSFS_ROOT, S_IFCHR),
	DEVFS_DEVICE("random", random, 0, 0, 0, 0666, SYSFS_ROOT, S_IFCHR),
	#endif
	//MCU peripherals
	DEVFS_DEVICE("core", mcu_core, 0, 0, 0, 0666, SYSFS_ROOT, S_IFCHR),
	DEVFS_DEVICE("core0", mcu_core, 0, 0, 0, 0666, SYSFS_ROOT, S_IFCHR),
	DEVFS_DEVICE("stdio-out", fifo, 0, &stdio_out_config, &stdio_out_state, 0666, SYSFS_ROOT, S_IFCHR),
	DEVFS_DEVICE("stdio-in", fifo, 0, &stdio_in_config, &stdio_in_state, 0666, SYSFS_ROOT, S_IFCHR),
	DEVFS_DEVICE("link-phy-usb", usbfifo, SOS_BOARD_USB_PORT, LINK_CONFIG, &sos_link_transport_usb_fifo_state, 0666, SOS_USER_ROOT, S_IFCHR),
	DEVFS_DEVICE("sys", sys, 0, 0, 0, 0666, SYSFS_ROOT, S_IFCHR),

	DEVFS_DEVICE("drive0", drive_cfi_qspi, 0, &drive0_cfi_config, &drive0_cfi_state, 0666, SYSFS_ROOT, S_IFBLK),
	DEVFS_DEVICE("drive1", drive_cfi_qspi, 0, &drive1_cfi_config, &drive1_cfi_state, 0666, SYSFS_ROOT, S_IFBLK),

	#if _IS_BOOT
	DEVFS_DEVICE("ramdrive", drive_ram, 0, &drive_ram_config, 0, 0666, SOS_USER_ROOT, S_IFBLK),
	#else
	DEVFS_DEVICE("drive2", drive_sdio_dma, 1, &sdio_configuration, &sdio_state, 0666, SOS_USER_ROOT, S_IFBLK),
	#endif

	#if !_IS_BOOT
	DEVFS_DEVICE("display0", display_device, 0, 0, 0, 0666, SYSFS_USER, S_IFCHR),

	//crypto
	DEVFS_DEVICE("hash0", mcu_hash, 0, 0, 0, 0666, SYSFS_USER, S_IFCHR),
	//DEVFS_DEVICE("crypt", mcu_hash, 0, 0, 0, 0666, SYSFS_USER, S_IFCHR),
	//DEVFS_DEVICE("random", mcu_hash, 0, 0, 0, 0666, SYSFS_USER, S_IFCHR),

	DEVFS_DEVICE("i2c0", mcu_i2c, 0, &i2c0_config, 0, 0666, SYSFS_USER, S_IFCHR), //PB8 and PB9 - I2C1
	DEVFS_DEVICE("i2c1", mcu_i2c, 1, &i2c1_config, 0, 0666, SYSFS_USER, S_IFCHR),
	DEVFS_DEVICE("i2c2", mcu_i2c, 2, &i2c2_config, 0, 0666, SYSFS_ROOT, S_IFCHR), //internal -- root access
	DEVFS_DEVICE("i2c3", mcu_i2c, 3, 0, 0, 0666, SYSFS_ROOT, S_IFCHR), //PB8 and PB9 and also I2C4

	DEVFS_DEVICE("i2s0", mcu_i2c, 3, 0, 0, 0666, SYSFS_ROOT, S_IFCHR), //PE3, PE4, PE5, PE6 SAI1/SAI4 A/B full duplex

	DEVFS_DEVICE("spi2", mcu_spi, 2, &spi2_dma_config, 0, 0666, SYSFS_ROOT, S_IFCHR), //GPIOH
	DEVFS_DEVICE("spi3", mcu_spi, 3, &spi3_dma_config, 0, 0666, SYSFS_USER, S_IFCHR), //GPIOH

	DEVFS_DEVICE("uart0", mcu_uart, 0, 0, 0, 0666, SYSFS_USER, S_IFCHR), //PA10 USART1 RX ONLY
	DEVFS_DEVICE("uart1", mcu_uart, 1, 0, 0, 0666, SYSFS_USER, S_IFCHR), //PA2, PA3 UART2
	DEVFS_DEVICE("uart3", mcu_uart, 3, 0, 0, 0666, SYSFS_USER, S_IFCHR), //PB8, PB9 UART4

	DEVFS_DEVICE("uart5", mcu_uart, 5, 0, 0, 0666, SYSFS_ROOT, S_IFCHR), //PC6, PC7 USART6

	#endif

	DEVFS_DEVICE("pio0", mcu_pio, 0, 0, 0, 0666, SYSFS_ROOT, S_IFCHR), //GPIOA
	DEVFS_DEVICE("pio1", mcu_pio, 1, 0, 0, 0666, SYSFS_ROOT, S_IFCHR), //GPIOB
	DEVFS_DEVICE("pio2", mcu_pio, 2, 0, 0, 0666, SYSFS_ROOT, S_IFCHR), //GPIOC
	DEVFS_DEVICE("pio3", mcu_pio, 3, 0, 0, 0666, SYSFS_ROOT, S_IFCHR), //GPIOD
	DEVFS_DEVICE("pio4", mcu_pio, 4, 0, 0, 0666, SYSFS_ROOT, S_IFCHR), //GPIOE
	DEVFS_DEVICE("pio5", mcu_pio, 5, 0, 0, 0666, SYSFS_ROOT, S_IFCHR), //GPIOF
	DEVFS_DEVICE("pio6", mcu_pio, 6, 0, 0, 0666, SYSFS_ROOT, S_IFCHR), //GPIOG
	DEVFS_DEVICE("pio7", mcu_pio, 7, 0, 0, 0666, SYSFS_ROOT, S_IFCHR), //GPIOH


	DEVFS_DEVICE("tmr0", mcu_tmr, 0, 0, 0, 0666, SYSFS_USER, S_IFCHR), //TIM1
	DEVFS_DEVICE("tmr1", mcu_tmr, 1, 0, 0, 0666, SYSFS_USER, S_IFCHR), //TIM2
	DEVFS_DEVICE("tmr2", mcu_tmr, 2, 0, 0, 0666, SYSFS_USER, S_IFCHR),
	DEVFS_DEVICE("tmr3", mcu_tmr, 3, 0, 0, 0666, SYSFS_USER, S_IFCHR),
	DEVFS_DEVICE("tmr4", mcu_tmr, 4, 0, 0, 0666, SYSFS_ROOT, S_IFCHR),
	DEVFS_DEVICE("tmr5", mcu_tmr, 5, 0, 0, 0666, SYSFS_ROOT, S_IFCHR),
	DEVFS_DEVICE("tmr6", mcu_tmr, 6, 0, 0, 0666, SYSFS_ROOT, S_IFCHR),
	DEVFS_DEVICE("tmr7", mcu_tmr, 7, 0, 0, 0666, SYSFS_ROOT, S_IFCHR), //TIM8

	DEVFS_TERMINATOR
};



