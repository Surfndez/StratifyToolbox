
#include <sos/dev/i2c.h>
#include <mcu/debug.h>
#include <pthread.h>
#include <fcntl.h>

#include "kernel_shared.h"

kernel_shared_t m_kernel_shared;
kernel_shared_root_t m_kernel_shared_root MCU_SYS_MEM;

void kernel_shared_root_set_pin_state(
      u8 pin_number,
      u8 peripheral_function,
      u8 io_flags
      ){
   m_kernel_shared_root.pin_state[pin_number].peripheral_function = peripheral_function;
   m_kernel_shared_root.pin_state[pin_number].io_flags = io_flags;
}

const kernel_shared_pin_state_t * kernel_shared_get_pin_state(
      u8 pin_number
      ){
   return m_kernel_shared_root.pin_state + pin_number;
}

sysfs_file_t * kernel_shared_i2c_file(){
   return &m_kernel_shared.i2c_file;
}

pthread_mutex_t * kernel_shared_i2c_mutex(){
   return &m_kernel_shared.i2c_mutex;
}

int kernel_shared_init(){
	int result;
	pthread_mutexattr_t mutex_attr;

	pthread_mutexattr_init(&mutex_attr);

	pthread_mutex_init(
				&m_kernel_shared.i2c_mutex,
				&mutex_attr
				);

	mcu_debug_log_info(MCU_DEBUG_USER0, "init kernel service");
	m_kernel_shared.i2c_file.fs = &(sysfs_list[1]);
	m_kernel_shared.i2c_file.flags = O_RDWR;
	m_kernel_shared.i2c_file.loc = 0;
	m_kernel_shared.i2c_file.handle = NULL;

	result = sysfs_file_open(
				&m_kernel_shared.i2c_file,
				"i2c2_internal",
				O_RDWR
				);

	if( result < 0 ){
		mcu_debug_log_error(
					MCU_DEBUG_USER0,
					"failed to init: internal I2C %d, %d)",
					result, errno
					);
		return -1;
	}

	mcu_debug_log_info(
				MCU_DEBUG_USER0,
				"set I2C attr"
				);
	result = sysfs_file_ioctl(&m_kernel_shared.i2c_file, I_I2C_SETATTR, 0);
	if( result < 0 ){
		mcu_debug_log_error(
					MCU_DEBUG_USER0,
					"failed to set attributes: internal I2C"
					);
		return -1;
	}

	return 0;
}
