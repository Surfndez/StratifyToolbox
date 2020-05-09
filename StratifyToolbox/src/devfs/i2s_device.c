
#include <mcu/pio.h>
#include <device/stream_ffifo.h>
#include "kernel/kernel_io.h"

#include "i2s_device.h"

int i2s_device_open(
      const devfs_handle_t * handle
      ){
   kernel_shared_root_reference_i2s();
   return stream_ffifo_open(handle);
}

int i2s_device_close(
      const devfs_handle_t * handle
      ){
   kernel_shared_root_dereference_i2s();
   return stream_ffifo_close(handle);
}

int i2s_device_ioctl(
      const devfs_handle_t * handle,
      int request,
      void * ctl
      ){
   //verify with gate that IO mode is enabled

   const i2s_attr_t * attributes = ctl;

   if( request == I_I2S_SETATTR ){
      int result = 0;

      if( attributes->o_flags & I2S_FLAG_SET_SLAVE ){
         result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel9, CORE_PERIPH_I2S, PIO_FLAG_SET_INPUT);
         result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel10, CORE_PERIPH_I2S, PIO_FLAG_SET_INPUT);
         result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel11, CORE_PERIPH_I2S, PIO_FLAG_SET_INPUT);
         result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel12, CORE_PERIPH_I2S, PIO_FLAG_SET_OUTPUT);
         if( result != 0 ){
            return SYSFS_SET_RETURN(EINVAL);
         }
      } else if( attributes->o_flags & I2S_FLAG_SET_MASTER ){
         result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel9, CORE_PERIPH_I2S, PIO_FLAG_SET_INPUT);
         result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel10, CORE_PERIPH_I2S, PIO_FLAG_SET_OUTPUT);
         result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel11, CORE_PERIPH_I2S, PIO_FLAG_SET_OUTPUT);
         result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel12, CORE_PERIPH_I2S, PIO_FLAG_SET_OUTPUT);
         if( result != 0 ){
            return SYSFS_SET_RETURN(EINVAL);
         }
      }
   }

   //this will be a streaming ffifo
   return stream_ffifo_ioctl(handle, request, ctl);
}

int i2s_device_read(
      const devfs_handle_t * handle,
      devfs_async_t * async){
   return stream_ffifo_read(handle, async);
}

int i2s_device_write(
      const devfs_handle_t * handle,
      devfs_async_t * async
      ){
   return stream_ffifo_write(handle, async);
}
