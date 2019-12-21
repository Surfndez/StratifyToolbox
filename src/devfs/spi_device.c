
#include <mcu/pio.h>
#include "kernel/kernel_io.h"

#include "spi_device.h"

int spi_device_open(
      const devfs_handle_t * handle
      ){
   kernel_shared_root_reference_spi();
   return mcu_spi_open(handle);
}

int spi_device_close(
      const devfs_handle_t * handle
      ){
   kernel_shared_root_dereference_spi();
   return mcu_spi_close(handle);
}

int spi_device_ioctl(
      const devfs_handle_t * handle,
      int request,
      void * ctl
      ){
   //verify with gate that IO mode is enabled

   if( request == I_SPI_SETATTR ){
      const spi_attr_t * attributes = ctl;

      int result = 0;

      if( attributes->o_flags & SPI_FLAG_SET_SLAVE ){
         result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel3, CORE_PERIPH_SPI, PIO_FLAG_SET_INPUT);
         result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel4, CORE_PERIPH_SPI, PIO_FLAG_SET_INPUT);
         result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel5, CORE_PERIPH_SPI, PIO_FLAG_SET_OUTPUT);
         result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel6, CORE_PERIPH_SPI, PIO_FLAG_SET_INPUT);
         if( result != 0 ){
            return SYSFS_SET_RETURN(EINVAL);
         }
      } else if( attributes->o_flags & SPI_FLAG_SET_MASTER ){
         result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel3, CORE_PERIPH_SPI, PIO_FLAG_SET_OUTPUT);
         result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel4, CORE_PERIPH_SPI, PIO_FLAG_SET_OUTPUT);
         result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel5, CORE_PERIPH_SPI, PIO_FLAG_SET_INPUT);
         result |= kernel_io_is_direction_assigned(kernel_shared_direction_channel6, CORE_PERIPH_SPI, PIO_FLAG_SET_OUTPUT);
         if( result != 0 ){
            return SYSFS_SET_RETURN(EINVAL);
         }
      }
   }
   return mcu_spi_ioctl(handle, request, ctl);
}

int spi_device_read(
      const devfs_handle_t * handle,
      devfs_async_t * async){
   return mcu_spi_read(handle, async);
}

int spi_device_write(
      const devfs_handle_t * handle,
      devfs_async_t * async
      ){
   return mcu_spi_write(handle, async);
}
