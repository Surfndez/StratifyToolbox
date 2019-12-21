
#include <mcu/pio.h>
#include "kernel/kernel_io.h"

#include "uart_device.h"

int uart_device_open(
      const devfs_handle_t * handle
      ){

   //only open if the pins have been acquired

   return SYSFS_SET_RETURN(EINVAL);
}

int uart_device_close(
      const devfs_handle_t * handle
      ){

   //if this is the final reference -- release the pins


   return SYSFS_SET_RETURN(EINVAL);
}

int uart_device_ioctl(
      const devfs_handle_t * handle,
      int request,
      void * ctl
      ){
   //verify with gate that IO mode is enabled
   return mcu_uart_ioctl(handle, request, ctl);
}

int uart_device_read(
      const devfs_handle_t * handle,
      devfs_async_t * async){
   return mcu_uart_read(handle, async);
}

int uart_device_write(
      const devfs_handle_t * handle,
      devfs_async_t * async
      ){
   return mcu_uart_write(handle, async);
}
