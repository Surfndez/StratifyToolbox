#ifndef USB_TOOLBOX_USB_CLASS_H
#define USB_TOOLBOX_USB_CLASS_H

#include <sos/link/transport_usb.h>
#include <device/usbfifo.h>
#include <usbd/control.h>
#include <usbd/cdc.h>

/* \details This structure defines the USB descriptors.  This
 * value is read over the control channel by the host to configure
 * the device.
 */
typedef struct MCU_PACK {
	usbd_configuration_descriptor_t configuration /* The configuration descriptor */;
	sos_link_interface_configuration_descriptor_t sos_link;
	usbd_cdc_configuration_descriptor_t vcp0; //uart
	usbd_cdc_configuration_descriptor_t vcp1; //GDB debugger
	u8 terminator  /* A null terminator used by the driver (required) */;
} toolbox_usb_configuration_descriptor_t;


#define TOOLBOX_USB_DESCRIPTOR_MANUFACTURER_SIZE 13
#define TOOLBOX_USB_DESCRIPTOR_PRODUCT_SIZE 16
#define TOOLBOX_USB_DESCRIPTOR_SERIAL_SIZE 16
#define TOOLBOX_USB_DESCRIPTOR_MANUFACTURER_STRING 'S','t','r','a','t','i','f','y',' ','L','a','b','s'
#define TOOLBOX_USB_DESCRIPTOR_PRODUCT_STRING 'S','t','r','a','t','i','f','y',' ','T','o','o','l','b','o','x'
#define TOOLBOX_USB_DESCRIPTOR_SERIAL_STRING '0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'

#define TOOLBOX_USB_DESCRIPTOR_VCP_0_SIZE 11
#define TOOLBOX_USB_DESCRIPTOR_VCP_0 'S','e','r','i','a','l',' ','P','o','r','t'
#define TOOLBOX_USB_DESCRIPTOR_VCP_1_SIZE 10
#define TOOLBOX_USB_DESCRIPTOR_VCP_1 'G','B','D','e','b','u','g','g','e','r'

#define TOOLBOX_USB_SOS_LINK_ENDPOINT_NUMBER 1
#define TOOLBOX_USB_SOS_LINK_ENDPOINT_SIZE 64
#define TOOLBOX_USB_VCP0_ENDPOINT_NUMBER 2 //uses 2 and 3
#define TOOLBOX_USB_VCP0_ENDPOINT_SIZE 64
#define TOOLBOX_USB_VCP1_ENDPOINT_NUMBER 4 //uses 4 and 5
#define TOOLBOX_USB_VCP1_ENDPOINT_SIZE 4


/*! \brief USB Link String Data
 * \details This structure defines the USB strings structure which includes
 * a string for the manufacturer, product, and serial number.
 */
struct MCU_PACK toolbox_usb_string_t {
	u8 bLength;
	u8 bDescriptorType;
	u16 wLANGID;
	usbd_declare_string(TOOLBOX_USB_DESCRIPTOR_MANUFACTURER_SIZE) manufacturer;
	usbd_declare_string(TOOLBOX_USB_DESCRIPTOR_PRODUCT_SIZE) product;
	usbd_declare_string(TOOLBOX_USB_DESCRIPTOR_SERIAL_SIZE) serial;
	usbd_declare_string(SOS_LINK_TRANSPORT_USB_DESCRIPTOR_SOS_LINK_SIZE) sos_link;
	usbd_declare_string(TOOLBOX_USB_DESCRIPTOR_VCP_0_SIZE) vcp0;
	usbd_declare_string(TOOLBOX_USB_DESCRIPTOR_VCP_1_SIZE) vcp1;
};

extern const usbd_device_descriptor_t toolbox_usb_device_desriptor;
extern const toolbox_usb_configuration_descriptor_t toolbox_usb_configuration_desriptor;
extern const struct toolbox_usb_string_t toolbox_usb_string_list;
extern const usbd_control_constants_t toolbox_usb_constants;

int toolbox_link_usbd_cdc_event_handler(void * context, const mcu_event_t * event);

//provided for the link device fifo
//DEVFS_DEVICE("link-phy-usb", 0, &toolbox_link_transport_usb_fifo_cfg, &toolbox_link_transport_usb_fifo_state, 0666, USER_ROOT, GROUP_ROOT),
extern const usbfifo_config_t toolbox_link_transport_usb_fifo_cfg;
extern usbfifo_state_t toolbox_link_transport_usb_fifo_state MCU_SYS_MEM;


extern const usbfifo_config_t toolbox_link_transport_usb_fifo_cfg;
extern usbfifo_state_t toolbox_link_transport_usb_fifo_state MCU_SYS_MEM;


#endif // USB_TOOLBOX_USB_CLASS_H
