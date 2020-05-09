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
#include <unistd.h>
#include <mcu/debug.h>
#include <sos/dev/pio.h>
#include "link_config.h"
#include "usb/toolbox_usb_class.h"

#define SOS_BOARD_USB_DP_PIN mcu_pin(0,12)
#define SOS_BOARD_USB_DM_PIN mcu_pin(0,11)

static link_transport_phy_t link_transport_open(const char * name, const void * options);

link_transport_driver_t link_transport = {
	.handle = -1,
	.open = link_transport_open,
	.read = sos_link_transport_usb_read,
	.write = sos_link_transport_usb_write,
	.close = sos_link_transport_usb_close,
	.wait = sos_link_transport_usb_wait,
	.flush = sos_link_transport_usb_flush,
	.timeout = 500,
	.transport_read = link2_transport_slaveread,
	.transport_write = link2_transport_slavewrite,
	.o_flags = 0

};

static usbd_control_t m_usb_control;

link_transport_phy_t link_transport_open(const char * name, const void * options){
	usb_attr_t usb_attr;
	link_transport_phy_t fd;
	MCU_UNUSED_ARGUMENT(options);

	//set up the USB attributes
	memset(&(usb_attr.pin_assignment), 0xff, sizeof(usb_pin_assignment_t));
	usb_attr.o_flags = USB_FLAG_SET_DEVICE; //| USB_FLAG_IS_VBUS_SENSING_ENABLED /*| USB_FLAG_IS_HIGH_SPEED */;
	usb_attr.pin_assignment.dp = SOS_BOARD_USB_DP_PIN;
	usb_attr.pin_assignment.dm = SOS_BOARD_USB_DM_PIN;
	//usb_attr.pin_assignment.vbus = mcu_pin(0,9); //not an alternate function (an additional function)
	usb_attr.freq = mcu_board_config.core_osc_freq;
	memset(usb_attr.tx_fifo_word_size, 0, USB_TX_FIFO_WORD_SIZE_COUNT);

	/* Total 4KB dedicated SRAM for FIFOs
	 *
	 * 512*4 = 2KB
	 * 32*4*6 = 768
	 *
	 */
	usb_attr.rx_fifo_word_size = 512; //RX fifo for all endpoints
	usb_attr.tx_fifo_word_size[0] = 64; //TX endpoint 0 -- Bulk IN
	usb_attr.tx_fifo_word_size[1] = 64; //TX endpoint 0 -- Bulk IN
	usb_attr.tx_fifo_word_size[2] = 32; //TX endpoint 1 -- Interrupt IN VCP0
	usb_attr.tx_fifo_word_size[3] = 64; //TX endpoint 2 -- BULK IN VCP0
	usb_attr.tx_fifo_word_size[4] = 32; //TX endpoint 3 -- Interrupt IN VCP1
	usb_attr.tx_fifo_word_size[5] = 64; //TX endpoint 4 -- Bulk IN VCP1

	mcu_debug_log_info(
				MCU_DEBUG_LINK,
				"Open USB"
				);

	fd = sos_link_transport_usb_open(
				name,
				&m_usb_control,
				&toolbox_usb_constants,
				&usb_attr,
				mcu_pin(0xff,0xff),
				1); //USB pin is active high

	mcu_debug_log_info(
				MCU_DEBUG_LINK,
				"USB Returned %d",
				fd
				);

	return fd;
}

