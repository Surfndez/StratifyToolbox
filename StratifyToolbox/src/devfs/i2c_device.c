
#include <mcu/i2c.h>
#include "kernel/kernel_io.h"

#include "i2c_device.h"

int i2c_device_open(
      const devfs_handle_t * handle
      ){

   kernel_shared_root_reference_i2c(handle->port);
   return mcu_i2c_open(handle);
}

int i2c_device_close(
      const devfs_handle_t * handle
      ){
   MCU_UNUSED_ARGUMENT(handle);
   return SYSFS_SET_RETURN(EINVAL);
}

int i2c_device_ioctl(
      const devfs_handle_t * handle,
      int request,
      void * ctl
      ){
   //verify with gate that IO mode is enabled

   if( request == I_I2C_SETATTR ){
      int result = 0;
      switch(handle->port){
         case 0:
            result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel1, CORE_PERIPH_I2C, PIO_FLAG_SET_INPUT);
            result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel2, CORE_PERIPH_I2C, PIO_FLAG_SET_INPUT);
            break;
         case 1:
            result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel3, CORE_PERIPH_I2C, PIO_FLAG_SET_INPUT);
            result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel4, CORE_PERIPH_I2C, PIO_FLAG_SET_INPUT);
            break;
      }
      if( result != 0 ){
         return SYSFS_SET_RETURN(EINVAL);
      }
   }

   return mcu_i2c_ioctl(handle, request, ctl);
}

int i2c_device_read(
      const devfs_handle_t * handle,
      devfs_async_t * async){
   return mcu_i2c_read(handle, async);
}

int i2c_device_write(
      const devfs_handle_t * handle,
      devfs_async_t * async
      ){
   return mcu_i2c_write(handle, async);
}
