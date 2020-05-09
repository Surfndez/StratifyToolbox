
#include <cortexm/cortexm.h>
#include <mcu/pio.h>

#include "ft5446.h"
#include "i2c_internal.h"

//Alert is PC13
#define ALERT_PORT 2
#define ALERT_PIN 13

enum {
	ft_slave_address = 0x38,
	ft_device_mode = 0x00,
	ft_reg_num_finger = 0x02,
	ft_tp1_reg = 0x03,
	ft_tp2_reg = 0x09,
	ft_id_g_lib_version = 0xa1,
	ft_id_g_mode = 0xa4,
	ft_id_g_thgroup = 0x80,
	ft_id_g_periodactive = 0x88,
	id_g_spec_gesture_enable = 0xd0
};

static int alert_init();
static int is_alert_asserted();
static void svcall_is_alert_asserted(void * args);
static void svcall_alert_init(void * args);

int ft5446_init(){
	alert_init();
	i2c_internal_lock(ft_slave_address);
	i2c_internal_write_register(ft_device_mode, 0x00);
	i2c_internal_write_register(ft_id_g_mode, 0x00);
	i2c_internal_write_register(ft_id_g_thgroup, 22);
	i2c_internal_write_register(ft_id_g_periodactive, 12);
	i2c_internal_unlock();
	return 0;
}

int ft5446_read(ft5446_packet_t * touch_packet){
	if( is_alert_asserted() ){
		i2c_internal_lock(ft_slave_address);
		i2c_internal_read(
					ft_reg_num_finger,
					touch_packet,
					sizeof(ft5446_packet_t)
					);
		i2c_internal_unlock();
		return touch_packet->count;
	}
	return 0;
}

void svcall_is_alert_asserted(void * args){
	CORTEXM_SVCALL_ENTER();
	int * result = args;
	u32 pinmask;
	devfs_handle_t handle = {0};
	handle.port = ALERT_PORT;
	mcu_pio_get(&handle, &pinmask);
	*result = (pinmask & (1<<ALERT_PIN)) == 0;
}

int is_alert_asserted(){
	int result;
	cortexm_svcall(svcall_is_alert_asserted, &result);
	return result;
}

void svcall_alert_init(void * args){
	CORTEXM_SVCALL_ENTER();
	devfs_handle_t handle = {0};
	pio_attr_t attributes;
	handle.port = ALERT_PORT;
	mcu_pio_open(&handle);
	attributes.o_flags = PIO_FLAG_SET_INPUT | PIO_FLAG_IS_PULLUP;
	attributes.o_pinmask = (1<<ALERT_PIN);
	mcu_pio_setattr(&handle, &attributes);
}

int alert_init(){
	cortexm_svcall(svcall_alert_init, 0);
	return 0;
}
