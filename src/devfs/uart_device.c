
#include <mcu/pio.h>
#include "kernel/kernel_io.h"

#include "uart_device.h"

int uart_device_open(
      const devfs_handle_t * handle
      ){

   //only open if the pins have been acquired
   kernel_shared_root_reference_uart(handle->port);
   return mcu_uart_open(handle);
}

int uart_device_close(
      const devfs_handle_t * handle
      ){

   //if this is the final reference -- release the pins
   kernel_shared_root_dereference_uart(handle->port);
   return mcu_uart_close(handle);
}

int uart_device_ioctl(
      const devfs_handle_t * handle,
      int request,
      void * ctl
      ){

   if( request == I_UART_SETATTR ){
      int result = 0;
      switch(handle->port){
         case 0:
            result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel13, CORE_PERIPH_UART, PIO_FLAG_SET_INPUT);
            break;
         case 1:
            result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel8, CORE_PERIPH_UART, PIO_FLAG_SET_OUTPUT);
            result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel3, CORE_PERIPH_UART, PIO_FLAG_SET_INPUT);
            break;
         case 3:
            result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel2, CORE_PERIPH_UART, PIO_FLAG_SET_OUTPUT);
            result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel1, CORE_PERIPH_UART, PIO_FLAG_SET_INPUT);
            break;
         case 5:
            result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel5, CORE_PERIPH_UART, PIO_FLAG_SET_OUTPUT);
            result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel6, CORE_PERIPH_UART, PIO_FLAG_SET_INPUT);
            break;
      }
      if( result != 0 ){
         return SYSFS_SET_RETURN(EINVAL);
      }
   }

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
