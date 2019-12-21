
#include <mcu/pio.h>
#include "kernel/kernel_io.h"

#include "spi_device.h"

int spi_device_open(
      const devfs_handle_t * handle
      ){

   return SYSFS_SET_RETURN(EINVAL);
}

int spi_device_close(
      const devfs_handle_t * handle
      ){
   MCU_UNUSED_ARGUMENT(handle);
   return SYSFS_SET_RETURN(EINVAL);
}

int spi_device_ioctl(
      const devfs_handle_t * handle,
      int request,
      void * ctl
      ){
   //verify with gate that IO mode is enabled
}

int spi_device_read(
      const devfs_handle_t * handle,
      devfs_async_t * async){
   return SYSFS_SET_RETURN(ENOTSUP);
}

int spi_device_write(
      const devfs_handle_t * handle,
      devfs_async_t * async
      ){
   return SYSFS_SET_RETURN(ENOTSUP);
}
