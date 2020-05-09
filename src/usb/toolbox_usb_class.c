

#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <sys/fcntl.h>
#include <unistd.h>

#include <sos/link/transport_usb.h>

#include "toolbox_usb_class.h"

static int cdc_event_handler(usbd_control_t * context, const mcu_event_t * event);

#define TOOLBOX_TRANSPORT_USB_DECLARE_INTERFACE_CONFIGURATION_DESCRIPTOR(interface_string,interface_number,endpoint_number,endpoint_size) \
	.interface_data = { \
		.bLength = sizeof(usbd_interface_descriptor_t), \
		.bDescriptorType = USBD_DESCRIPTOR_TYPE_INTERFACE, \
		.bInterfaceNumber = interface_number, \
		.bAlternateSetting = 0x00, \
		.bNumEndpoints = 0x02, \
		.bInterfaceClass = 0xff, \
		.bInterfaceSubClass = 0xff, \
		.bInterfaceProtocol = 0xff, \
		.iInterface = interface_string \
	}, \
	.data_out = { \
		.bLength= sizeof(usbd_endpoint_descriptor_t), \
		.bDescriptorType=USBD_DESCRIPTOR_TYPE_ENDPOINT, \
		.bEndpointAddress=endpoint_number, \
		.bmAttributes=USBD_ENDPOINT_ATTRIBUTES_TYPE_BULK, \
		.wMaxPacketSize=endpoint_size, \
		.bInterval=1 \
	}, \
	.data_in = { \
		.bLength= sizeof(usbd_endpoint_descriptor_t), \
		.bDescriptorType=USBD_DESCRIPTOR_TYPE_ENDPOINT, \
		.bEndpointAddress=(endpoint_number)|0x80, \
		.bmAttributes=USBD_ENDPOINT_ATTRIBUTES_TYPE_BULK, \
		.wMaxPacketSize=endpoint_size, \
		.bInterval=1 \
	}

#define SOS_USBD_VID 0x20A0
#define SOS_USBD_PID 0x413B

#define SOS_REQUIRED_CURRENT 500
#define USB0_DEVFIFO_BUFFER_SIZE 64

#define TOOLBOX_TOOLBOX_LINK_INTERFACE_NUMBER 0
#define TOOLBOX_VCP0_INTERFACE_NUMBER 1
#define TOOLBOX_VCP0_DATA_INTERFACE_NUMBER 2
#define TOOLBOX_VCP1_INTERFACE_NUMBER 3
#define TOOLBOX_VCP1_DATA_INTERFACE_NUMBER 4

#define TOOLBOX_TOOLBOX_LINK_INTERFACE_STRING 4
#define TOOLBOX_VCP0_INTERFACE_STRING 5
#define TOOLBOX_VCP1_INTERFACE_STRING 6

/*! \details This variable stores the device descriptor.  It has a weak attribute and can be
 * overridden by using a user specific value
 * in the file \a devices.c.  This allows the user to change the USB vendor and product IDs.
 * All other values should be unchanged.
 *
 */
const usbd_device_descriptor_t toolbox_usb_device_desriptor = {
	.bLength = sizeof(usbd_device_descriptor_t),
	.bDescriptorType = USBD_DESCRIPTOR_TYPE_DEVICE,
	.bcdUSB = 0x0200,
	.bDeviceClass = 0, //Composite device (2 VCPs)
	.bDeviceSubClass = 0,
	.bDeviceProtocol = 0,
	.bMaxPacketSize = MCU_CORE_USB_MAX_PACKET_ZERO_VALUE,
	.idVendor = SOS_USBD_VID,
	.idProduct = SOS_USBD_PID,
	.bcdDevice = 0x200,
	.iManufacturer = 1,
	.iProduct = 2,
	.iSerialNumber = 3,
	.bNumConfigurations = 1
};

const toolbox_usb_configuration_descriptor_t toolbox_usb_configuration_desciptor = {

	.configuration = {
		.bLength = sizeof(usbd_configuration_descriptor_t),
		.bDescriptorType = USBD_DESCRIPTOR_TYPE_CONFIGURATION,

		.wTotalLength = sizeof(toolbox_usb_configuration_desciptor)-1, //exclude the zero terminator
		.bNumInterfaces = 0x04,
		.bConfigurationValue = 0x01,
		.iConfiguration = 0x03,
		.bmAttributes = USBD_CONFIGURATION_ATTRIBUTES_BUS_POWERED,
		.bMaxPower = USBD_CONFIGURATION_MAX_POWER_MA( SOS_REQUIRED_CURRENT )
	},

	.sos_link = {
		TOOLBOX_TRANSPORT_USB_DECLARE_INTERFACE_CONFIGURATION_DESCRIPTOR(
		TOOLBOX_TOOLBOX_LINK_INTERFACE_STRING,
		TOOLBOX_TOOLBOX_LINK_INTERFACE_NUMBER,
		TOOLBOX_USB_SOS_LINK_ENDPOINT_NUMBER,
		TOOLBOX_USB_SOS_LINK_ENDPOINT_SIZE
		)
	},

	.vcp0 = {
		USBD_CDC_DECLARE_CONFIGURATION_DESCRIPTOR(
		TOOLBOX_VCP0_INTERFACE_STRING,
		TOOLBOX_VCP0_INTERFACE_STRING,
		TOOLBOX_VCP0_INTERFACE_NUMBER,
		TOOLBOX_VCP0_INTERFACE_NUMBER+1,
		TOOLBOX_USB_VCP0_ENDPOINT_NUMBER,
		64,
		TOOLBOX_USB_VCP0_ENDPOINT_NUMBER+1,
		64
		)
	},

	.vcp1 = {
		USBD_CDC_DECLARE_CONFIGURATION_DESCRIPTOR(
		TOOLBOX_VCP1_INTERFACE_STRING,
		TOOLBOX_VCP1_INTERFACE_STRING,
		TOOLBOX_VCP1_INTERFACE_NUMBER,
		TOOLBOX_VCP1_INTERFACE_NUMBER+1,
		TOOLBOX_USB_VCP1_ENDPOINT_NUMBER,
		64,
		TOOLBOX_USB_VCP1_ENDPOINT_NUMBER+1,
		64
		)
	},

	.terminator = 0
};

/*! \details This variable stores the USB strings as the defaults listed below:
 * - Manufacturer: "Stratify OS, Inc"
 * - Product: "Stratify OS"
 * - Serial Number: "00000000"
 *
 * This variable has a weak attribute.  It can be overridden by using a user specific value
 * is the file \a devices.c.
 *
 */
const struct toolbox_usb_string_t toolbox_usb_string_list = {
	.bLength = 4,
	.bDescriptorType = USBD_DESCRIPTOR_TYPE_STRING,
	.wLANGID = 0x0409, //English
	.manufacturer = usbd_assign_string(TOOLBOX_USB_DESCRIPTOR_MANUFACTURER_SIZE, TOOLBOX_USB_DESCRIPTOR_MANUFACTURER_STRING),
	.product = usbd_assign_string(TOOLBOX_USB_DESCRIPTOR_PRODUCT_SIZE, TOOLBOX_USB_DESCRIPTOR_PRODUCT_STRING),
	.serial = usbd_assign_string(TOOLBOX_USB_DESCRIPTOR_SERIAL_SIZE, 0), //dynamically load SN based on silicon
	.sos_link = usbd_assign_string(SOS_LINK_TRANSPORT_USB_DESCRIPTOR_SOS_LINK_SIZE, SOS_LINK_TRANSPORT_USB_DESCRIPTOR_SOS_LINK),
	.vcp0 = usbd_assign_string(TOOLBOX_USB_DESCRIPTOR_VCP_0_SIZE, TOOLBOX_USB_DESCRIPTOR_VCP_0),
	.vcp1 = usbd_assign_string(TOOLBOX_USB_DESCRIPTOR_VCP_1_SIZE, TOOLBOX_USB_DESCRIPTOR_VCP_1)
};


int toolbox_usb_cdc_event_handler(void * object, const mcu_event_t * event){
	usbd_control_t * context = object;

	//if this is a class request check the CDC interfaces
	if ( usbd_control_setup_request_type(context) == USBD_REQUEST_TYPE_CLASS ){
		return cdc_event_handler(context, event);
	}

	return 0;
}

const usbd_control_constants_t toolbox_usb_constants = {
	.handle.port = 0,
	.handle.config = 0,
	.handle.state = 0,
	.device =  &toolbox_usb_device_desriptor,
	.config = &toolbox_usb_configuration_desciptor,
	.string = &toolbox_usb_string_list,
	.class_event_handler = toolbox_usb_cdc_event_handler,
};

int cdc_event_handler(usbd_control_t * context, const mcu_event_t * event){
	u32 rate = 12000000;
	u32 o_events = event->o_events;
	int iface = usbd_control_setup_interface(context);

	if( (iface == 0) || (iface == 1) || (iface == 2) || (iface == 3) ){

		if ( (o_events & MCU_EVENT_FLAG_SETUP) ){
			switch(context->setup_packet.bRequest){
				case USBD_CDC_REQUEST_SEND_ENCAPSULATED_COMMAND:
				case USBD_CDC_REQUEST_SET_COMM_FEATURE:
				case USBD_CDC_REQUEST_SEND_BREAK:
					//need to receive information from the host
					usbd_control_prepare_buffer(context); //received the incoming data in the buffer
					usbd_control_statusin_stage(context); //data out stage?
					return 1;

				case USBD_CDC_REQUEST_SET_LINE_CODING:
					//usbd_control_statusin_stage(context); //data out stage?
					context->data.dptr = context->buf;
					context->data.nbyte = context->setup_packet.wLength;
					return 1;

				case USBD_CDC_REQUEST_SET_CONTROL_LINE_STATE:
					usbd_control_statusin_stage(context);
					return 1;

				case USBD_CDC_REQUEST_GET_LINE_CODING:
					context->data.dptr = context->buf;

					//copy line coding to dev_std_buf
					context->buf[0] = (rate >>  0) & 0xFF;
					context->buf[1] = (rate >>  8) & 0xFF;
					context->buf[2] = (rate >> 16) & 0xFF;
					context->buf[3] = (rate >> 24) & 0xFF;  //rate
					context->buf[4] =  0; //stop bits 1
					context->buf[5] =  0; //no parity
					context->buf[6] =  8; //8 data bits
					usbd_control_datain_stage(context);
					return 1;

				case USBD_CDC_REQUEST_CLEAR_COMM_FEATURE:
					usbd_control_statusin_stage(context);
					return 1;

				case USBD_CDC_REQUEST_GET_COMM_FEATURE:
					context->data.dptr = context->buf;
					//copy data to dev_std_buf
					usbd_control_statusin_stage(context);
					return 1;

				case USBD_CDC_REQUEST_GET_ENCAPSULATED_RESPONSE:
					context->data.dptr = context->buf;
					//copy data to dev_std_buf
					usbd_control_statusin_stage(context);
					return 1;

				default:
					return 0;
			}

		} else if ( o_events & MCU_EVENT_FLAG_DATA_READY ){
			switch(context->setup_packet.bRequest){
				case USBD_CDC_REQUEST_SET_LINE_CODING:
					//line coding info is available in context->buf
					usbd_control_statusin_stage(context);
					return 1;

				case USBD_CDC_REQUEST_SET_CONTROL_LINE_STATE:
				case USBD_CDC_REQUEST_SET_COMM_FEATURE:
				case USBD_CDC_REQUEST_SEND_ENCAPSULATED_COMMAND:
					//use data in dev_std_buf to take action
					usbd_control_statusin_stage(context);
					return 1;
				default:
					return 0;
			}
		}
	}
	//The request was not handled
	return 0;
}

