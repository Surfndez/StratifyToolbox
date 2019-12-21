
#include <mcu/pio.h>
#include "kernel/kernel_io.h"
#include "pio_device.h"

int pio_device_open(
      const devfs_handle_t * handle
      ){

   return 0;
}

int pio_device_close(
      const devfs_handle_t * handle
      ){
   MCU_UNUSED_ARGUMENT(handle);
   return SYSFS_SET_RETURN(EINVAL);
}

int pio_device_ioctl(
      const devfs_handle_t * handle,
      int request,
      void * ctl
      ){
   //verify with gate that IO mode is enabled
   const pio_attr_t * attributes = ctl;
   int result;

   switch(request){
      case I_PIO_SETATTR:
         //can only set pins as output if they are outputs on the kernel_shared settings
         for(enum kernel_shared_direction_channels i= kernel_shared_direction_channel_first;
             i < kernel_shared_direction_channel_last+1;
             i++){
            if( (1<<i) & attributes->o_pinmask ){
               result = kernel_io_is_direction_assigned(
                        i,
                        CORE_PERIPH_PIO,
                        attributes->o_flags
                        );
               if( result < 0 ){
                  return result;
               }
               if( result == 0 ){
                  return SYSFS_SET_RETURN(EBUSY);
               }
            }
         }
         //if it passes then this pin is operation is good to go
         break;
   }

   return mcu_pio_ioctl(handle, request, ctl);
}


int pio_device_read(
      const devfs_handle_t * handle,
      devfs_async_t * async){
   return SYSFS_SET_RETURN(ENOTSUP);
}

int pio_device_write(
      const devfs_handle_t * handle,
      devfs_async_t * async
      ){
   return SYSFS_SET_RETURN(ENOTSUP);
}
