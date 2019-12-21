/**
 *
 * \file
 *
 * \brief This module contains SAMD21 BSP APIs implementation.
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include <unistd.h>
#include <sos/sos.h>

#include "bsp/include/nm_bsp.h"
#include "common/include/nm_common.h"
#include "devfs/wifi_phy_device.h"
#include "kernel/wifi_api.h"

/*
 *	@fn		nm_bsp_init
 *	@brief	Initialize BSP
 *	@return	0 in case of success and -1 in case of failure
 *	@author	M.S.M
 *	@date	11 July 2012
 *	@version	1.0
 */
sint8 nm_bsp_init(void)
{
	//initialize pins
	const wifi_api_config_t * config =
			sos_board_config.socket_api->config;

	MCU_DEBUG_LINE_TRACE();

	int result;
	if( (result =
		  sysfs_shared_open(
			  &config->device_config
			  )
		  ) < 0 ){
		mcu_debug_log_error(
					MCU_DEBUG_SOCKET,
					"Failed to initialize WIFI phy device (%d, %d)",
					result,
					errno
					);
		return M2M_ERR_FAIL;
	}


	nm_bsp_reset();

	return M2M_SUCCESS;
}

/**
 *	@fn		nm_bsp_deinit
 *	@brief	De-iInitialize BSP
 *	@return	0 in case of success and -1 in case of failure
 *	@author	M. Abdelmawla
 *	@date	11 July 2012
 *	@version	1.0
 */
sint8 nm_bsp_deinit(void)
{

	return M2M_SUCCESS;
}

/**
 *	@fn		nm_bsp_reset
 *	@brief	Reset NMC1500 SoC by setting CHIP_EN and RESET_N signals low,
 *           CHIP_EN high then RESET_N high
 *	@author	M. Abdelmawla
 *	@date	11 July 2012
 *	@version	1.0
 */
void nm_bsp_reset(void){
	//toggle the HW reset pin
	const wifi_api_config_t * config = sos_board_config.socket_api->config;

	mcu_debug_log_info(
				MCU_DEBUG_SOCKET,
				"reset wifi chip"
				);
	int result = sysfs_shared_ioctl(
				&config->device_config,
				I_WIFIPHY_ASSERT_RESET,
				0
				);
	if( result < 0 ){
		mcu_debug_log_error(
					MCU_DEBUG_SOCKET,
					"Failed to assert wifi phy reset (%d, %d)",
					result,
					errno
					);
	}

	usleep(10*1000);

	result = sysfs_shared_ioctl(
				&config->device_config,
				I_WIFIPHY_DEASSERT_RESET,
				0
				);

	if( result < 0 ){
		mcu_debug_log_error(
					MCU_DEBUG_SOCKET,
					"Failed to deassert wifi phy reset (%d, %d)",
					result,
					errno
					);
	}
}

/*
 *	@fn		nm_bsp_sleep
 *	@brief	Sleep in units of mSec
 *	@param[IN]	u32TimeMsec
 *				Time in milliseconds
 *	@author	M.S.M
 *	@date	28 OCT 2013
 *	@version	1.0
 */
void nm_bsp_sleep(uint32 u32TimeMsec){
	usleep(u32TimeMsec*1000UL);
}

/*
 *	@fn		nm_bsp_register_isr
 *	@brief	Register interrupt service routine
 *	@param[IN]	pfIsr
 *				Pointer to ISR handler
 *	@author	M.S.M
 *	@date	28 OCT 2013
 *	@sa		tpfNmBspIsr
 *	@version	1.0
 */
void nm_bsp_register_isr(tpfNmBspIsr pfIsr){

}

/*
 *	@fn		nm_bsp_interrupt_ctrl
 *	@brief	Enable/Disable interrupts
 *	@param[IN]	u8Enable
 *				'0' disable interrupts. '1' enable interrupts
 *	@author	M.S.M
 *	@date	28 OCT 2013
 *	@version	1.0
 */
void nm_bsp_interrupt_ctrl(uint8 u8Enable){

}

int nm_bsp_toolbox_spi_read_write(
		uint8* pu8Mosi, //in buf master output
		uint8* pu8Miso, //out buf master input
		uint16 u16Sz
		){
	const wifi_api_config_t * config = sos_board_config.socket_api->config;
	int result;

	if( (pu8Miso != 0) &&
		 (pu8Mosi != 0) ){

		sysfs_shared_ioctl(
					&config->device_config,
					I_WIFIPHY_SET_FULL_DUPLEX,
					0
					);

		struct aiocb aio_read_buffer;
		memset(&aio_read_buffer,
				 0,
				 sizeof(aio_read_buffer));

		aio_read_buffer.aio_offset = 0;
		aio_read_buffer.aio_sigevent.sigev_notify = SIGEV_NONE;
		aio_read_buffer.aio_buf = pu8Miso;
		aio_read_buffer.aio_nbytes = u16Sz;
		aio_read_buffer.aio_lio_opcode = LIO_READ;

		//queue the read
		sysfs_shared_aio(
					&config->device_config,
					&aio_read_buffer
					);
	} else {
		sysfs_shared_ioctl(
					&config->device_config,
					I_WIFIPHY_SET_HALF_DUPLEX,
					0
					);
	}

	//execute the full duplex transfer
	result = -1;
	if( pu8Mosi ){ //master output

		result = sysfs_shared_write(
					&config->device_config,
					0,
					pu8Mosi,
					u16Sz
					);

	} else if( pu8Miso ){ //master input

		result = sysfs_shared_read(
					&config->device_config,
					0,
					pu8Miso,
					u16Sz
					);
	}

	if( result < 0 ){
		mcu_debug_printf(
					"spi failed %d, %d\n",
					result,
					errno
					);
		return M2M_ERR_BUS_FAIL;
	}

#if 1
	for(int i=0; i < u16Sz; i++){
		mcu_debug_printf("SPI[%d]: ", i);
		if( pu8Mosi ){
			mcu_debug_printf("->%02X ", pu8Mosi[i]);
		}

		if( pu8Miso ){
			mcu_debug_printf("<-%02X", pu8Miso[i]);
		}
		mcu_debug_printf("\n");
	}
#endif

	//both operations end at the same time
	return M2M_SUCCESS;
}

