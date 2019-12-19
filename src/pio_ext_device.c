

#include "pio_ext_device.h"

int pio_ext_device_open(
      const devfs_handle_t * handle
      ){

}

int pio_ext_device_close(
      const devfs_handle_t * handle
      ){

}

int pio_ext_device_ioctl(
      const devfs_handle_t * handle,
      int request,
      void * ctl
      ){

   const pio_attr_t * attributes = ctl;

   switch(request){
      case I_PIO_GETVERSION: return 0;
      case I_PIO_GETINFO:
      case I_PIO_SETATTR:
         //use the I2C devices to set the direction
         //PIO_FLAG_SET_INPUT, PIO_FLAG_SET_OUTPUT
         //

         break;
      case I_PIO_SETACTION:
      case I_PIO_SETMASK:
      case I_PIO_CLRMASK:
      case I_PIO_GET:

         //read the underlying GPIO pins

         break;
      case I_PIO_SET:
         //
         break;
   }

   return 0;
}

int pio_ext_device_read(
      const devfs_handle_t * handle,
      devfs_async_t * async){
   return SYSFS_SET_RETURN(ENOTSUP);
}

int pio_ext_device_write(
      const devfs_handle_t * handle,
      devfs_async_t * async
      ){
   return SYSFS_SET_RETURN(ENOTSUP);
}
