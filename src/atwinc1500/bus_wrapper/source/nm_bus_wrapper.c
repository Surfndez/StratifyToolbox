/**
 *
 * \file
 *
 * \brief This module contains NMC1000 bus wrapper APIs implementation.
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


#include <mcu/types.h>
#include <sos/fs/sysfs.h>
#include <sos/dev/spi.h>
#include <string.h>
#include <fcntl.h>
#include <aio.h>
#include "bus_wrapper/include/nm_bus_wrapper.h"

sysfs_file_t m_spi_file;

tstrNmBusCapabilities egstrNmBusCapabilities = {
	.u16MaxTrxSz = 512
};


static sint8 spi_rw(
		uint8* pu8Mosi,
		uint8* pu8Miso,
		uint16 u16Sz
		){
	struct aiocb aio_read_buffer;

	memset(&aio_read_buffer,
				0,
				sizeof(aio_read_buffer));

	aio_read_buffer.aio_offset = 0;
	//aio_read_buffer.aio_sigevent = 0; //?
	aio_read_buffer.aio_buf = pu8Miso;
	aio_read_buffer.aio_nbytes = u16Sz;
	aio_read_buffer.aio_lio_opcode = LIO_READ;

	//queue the read
	sysfs_file_aio(&m_spi_file, &aio_read_buffer);

	//execute the full duplex transfer
	if( sysfs_file_write(&m_spi_file, pu8Mosi, u16Sz) != u16Sz ){
		return M2M_ERR_BUS_FAIL;
	}

	//both operations end at the same time
	return M2M_SUCCESS;
}

/*
*	@fn		nm_bus_init
*	@brief	Initialize the bus wrapper
*	@return	M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@author	M.S.M
*	@date	28 oct 2013
*	@version	1.0
*/
sint8 nm_bus_init(void * pvInitValue){
	sint8 result = M2M_SUCCESS;

	int file_result;

	file_result	= sysfs_file_open(&m_spi_file, "/dev/spi0", O_RDWR);
	if( file_result < 0 ){
		//
	}

	file_result = sysfs_file_ioctl(
				&m_spi_file,
				I_SPI_SETATTR,
				0
				);
	if( file_result < 0 ){
		//
	}


	/* Reset WINC1500. */
	nm_bsp_reset();
	nm_bsp_sleep(1);

	return result;
}

/*
*	@fn		nm_bus_ioctl
*	@brief	send/receive from the bus
*	@param[IN]	u8Cmd
*					IOCTL command for the operation
*	@param[IN]	pvParameter
*					Arbitrary parameter depenging on IOCTL
*	@return	M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@author	M.S.M
*	@date	28 oct 2013
*	@note	For SPI only, it's important to be able to send/receive at the same time
*	@version	1.0
*/
sint8 nm_bus_ioctl(uint8 u8Cmd, void * pvParameter)
{
	sint8 s8Ret = 0;
	switch(u8Cmd)
	{
		case NM_BUS_IOCTL_RW: {
			tstrNmSpiRw *pstrParam = (tstrNmSpiRw *)pvParameter;
			s8Ret =
					spi_rw(
						pstrParam->pu8InBuf,
						pstrParam->pu8OutBuf,
						pstrParam->u16Sz
						);
		}
		break;
		default:
			s8Ret = -1;
			M2M_ERR("invalide ioclt cmd\n");
			break;
	}

	return s8Ret;
}

/*
*	@fn		nm_bus_deinit
*	@brief	De-initialize the bus wrapper
*	@author	M.S.M
*	@date	28 oct 2013
*	@version	1.0
*/
sint8 nm_bus_deinit(void)
{
	//close SPI
	return 0;
}

/*
*	@fn			nm_bus_reinit
*	@brief		re-initialize the bus wrapper
*	@param [in]	void *config
*					re-init configuration data
*	@return		M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@author		Dina El Sissy
*	@date		19 Sept 2012
*	@version	1.0
*/
sint8 nm_bus_reinit(void*  config ){
	MCU_UNUSED_ARGUMENT(config);
	return M2M_SUCCESS;
}


