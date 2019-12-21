
#include <mcu/pio.h>
#include "kernel/kernel_io.h"

#include "i2s_device.h"

int i2s_device_open(
      const devfs_handle_t * handle
      ){

   return SYSFS_SET_RETURN(EINVAL);
}

int i2s_device_close(
      const devfs_handle_t * handle
      ){
   MCU_UNUSED_ARGUMENT(handle);
   return SYSFS_SET_RETURN(EINVAL);
}

int i2s_device_ioctl(
      const devfs_handle_t * handle,
      int request,
      void * ctl
      ){
   //verify with gate that IO mode is enabled
}

int i2s_device_read(
      const devfs_handle_t * handle,
      devfs_async_t * async){
   return SYSFS_SET_RETURN(ENOTSUP);
}

int i2s_device_write(
      const devfs_handle_t * handle,
      devfs_async_t * async
      ){
   return SYSFS_SET_RETURN(ENOTSUP);
}
