
#include <sos/dev/i2c.h>
#include <fcntl.h>
#include <mcu/debug.h>

#include "i2c_internal.h"
#include "kernel_shared.h"

int i2c_internal_init(){

	pthread_mutexattr_t mutex_attr;
	pthread_mutexattr_init(&mutex_attr);
	pthread_mutexattr_setpshared(&mutex_attr, 1);
	pthread_mutex_init(
				kernel_shared_i2c_mutex(),
				&mutex_attr
				);

	kernel_shared_i2c_file()->fs = &(sysfs_list[SOS_BOARD_DEVFS_OFFSET]);
	kernel_shared_i2c_file()->flags = O_RDWR;
	kernel_shared_i2c_file()->loc = 0;
	kernel_shared_i2c_file()->handle = NULL;

	int result = sysfs_file_open(
				kernel_shared_i2c_file(),
				"i2c_internal",
				O_RDWR
				);

	if( result < 0 ){
		mcu_debug_log_error(
					MCU_DEBUG_USER0,
					"failed to init: internal I2C (%d, %d)",
					result, errno
					);
		return -1;
	}

	result = sysfs_file_ioctl(
				kernel_shared_i2c_file(),
				I_I2C_SETATTR,
				0
				);
	if( result < 0 ){
		mcu_debug_log_error(
					MCU_DEBUG_USER0,
					"failed to set attributes: internal I2C"
					);
		return -1;
	}

	return 0;
}

int i2c_internal_write_register(u8 address, u8 value){
	return i2c_internal_write(address, &value, 1);
}

u8 i2c_internal_read_register(u8 address){
	u8 result = 0;
	i2c_internal_write(address, &result, 1);
	return result;
}

int i2c_internal_write(
      int location,
      const void * buffer,
      int nbyte
      ){
   kernel_shared_i2c_file()->loc = location;
   return sysfs_file_write(
            kernel_shared_i2c_file(),
            buffer,
            nbyte
            );
}

int i2c_internal_read(int location, void * buffer, int nbyte){
   kernel_shared_i2c_file()->loc = location;
   return sysfs_file_read(
            kernel_shared_i2c_file(),
            buffer,
            nbyte
            );
}

int i2c_internal_lock(
      u8 slave_address
      ){
   int result;
   //lock mutex

   if( pthread_mutex_lock(
          kernel_shared_i2c_mutex()
          ) < 0 ){
      mcu_debug_log_error(
               MCU_DEBUG_USER0,
               "failed to lock mutex for 0x%X",
               slave_address
               );
      return -1;
   }

   i2c_attr_t attributes = {0};
   attributes.o_flags = I2C_FLAG_PREPARE_PTR_DATA;
   attributes.slave_addr[0].addr8[0] = slave_address;
   result = sysfs_file_ioctl(
            kernel_shared_i2c_file(),
            I_I2C_SETATTR,
            &attributes
            );
   if( result < 0 ){
      return -1;
   }

   return 0;
}

int i2c_internal_unlock(){

   if( pthread_mutex_unlock(
          kernel_shared_i2c_mutex()
          ) < 0 ){
      return -1;
   }
   //unlock mutex
   return 0;
}
