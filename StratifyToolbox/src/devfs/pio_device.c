
#include <mcu/pio.h>
#include "kernel/kernel_io.h"
#include "pio_device.h"

const mcu_pin_t io_mcu_pin_table[last_kernel_shared_direction_channel+1] = {
   { 0xff, 0xff }, //Doesn't exist -- start with pin1
   { 1, 8 }, //PB8
   { 1, 9 }, //PB9
   { 4, 11 }, //PE11
   { 4, 12 }, //PE12
   { 4, 13 }, //PE13
   { 4, 14 }, //PE14
   { 0xff, 0xff }, //Not connected -- ground pin
   { 0, 2 }, //PA2
   { 0, 3 }, //PA3
   { 0, 6 }, //PA6
   { 0, 7 }, //PA7
   { 0, 15 }, //PA15
   { 0, 10 }, //PA10
   { 0, 9 }, //PA9
   { 1, 13 }, //PB13
   { 3, 2 }, //PD2
   { 3, 3 }, //PD3
   { 3, 6 }, //PD6
   { 4, 0 } //PE0
};

static int setattr(u32 o_pinmask, u32 o_flags);
static int setmask(u32 o_pinmask);
static int clrmask(u32 o_pinmask);
static int set(u32 o_pinmask);
static int get(u32 * o_pinmask);

int pio_device_open(
      const devfs_handle_t * handle
      ){

   if( handle->port != 0 ){ return SYSFS_SET_RETURN(EINVAL); }

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
         for(enum kernel_shared_direction_channels i= first_kernel_shared_direction_channel;
             i < last_kernel_shared_direction_channel+1;
             i++){
            if( (1<<i) & attributes->o_pinmask ){
               result = kernel_io_is_direction_assigned(
                        i,
                        CORE_PERIPH_PIO,
                        attributes->o_flags
                        );
               if( result < 0 ){
                  mcu_debug_printf("Pin is busy with another peripheral\n");
                  return result;
               }
            }
         }


         //if it passes then this pin is operation is good to go
         //need to map the IO pin number with the MCU IO pin
         return setattr(attributes->o_pinmask, attributes->o_flags);

      case I_PIO_SETMASK:
         return setmask((u32)ctl);

      case I_PIO_CLRMASK:
         return clrmask((u32)ctl);

      case I_PIO_SET:
         return set((u32)ctl);

      case I_PIO_GET:
         return get(ctl);
   }

	 return SYSFS_SET_RETURN(EINVAL);
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


int setattr(u32 o_pinmask, u32 o_flags){
   mcu_pin_t pin;
   devfs_handle_t handle = {0};
   pio_attr_t attributes;
   attributes.o_flags = o_flags;
   for(u32 i=first_kernel_shared_direction_channel;
       i < last_kernel_shared_direction_channel+1;
       i++){
      pin = io_mcu_pin_table[i];
      if( (o_pinmask & (1<<i)) && (pin.port != 0xff) ){
         handle.port = pin.port;
         attributes.o_pinmask = (1<<pin.pin);
				 mcu_pio_setattr(&handle, &attributes);
      }
   }
   return 0;
}

int setmask(u32 o_pinmask){
   mcu_pin_t pin;
   devfs_handle_t handle = {0};
   for(u32 i=first_kernel_shared_direction_channel;
       i < last_kernel_shared_direction_channel+1;
       i++){
      pin = io_mcu_pin_table[i];
      if( (o_pinmask & (1<<i)) && (pin.port != 0xff) ){
         handle.port = pin.port;
         u32 pin_mask = 1<<pin.pin;
				 mcu_pio_setmask(&handle, (void*)pin_mask);
      }
   }
   return 0;
}

int clrmask(u32 o_pinmask){
   mcu_pin_t pin;
   devfs_handle_t handle = {0};
   for(u32 i=first_kernel_shared_direction_channel;
       i < last_kernel_shared_direction_channel+1;
       i++){
      pin = io_mcu_pin_table[i];
      if( (o_pinmask & (1<<i)) && (pin.port != 0xff) ){
         handle.port = pin.port;
         u32 pin_mask = 1<<pin.pin;
				 mcu_pio_clrmask(&handle, (void*)pin_mask);
      }
   }
   return 0;
}

int set(u32 o_pinmask){
   setmask(o_pinmask);
   clrmask(~o_pinmask);
   return 0;
}

int get(u32 * o_pinmask){
   mcu_pin_t pin;
   u32 result = 0;
   u32 tmp;
   devfs_handle_t handle = {0};
   for(u32 i=first_kernel_shared_direction_channel;
       i < last_kernel_shared_direction_channel+1;
       i++){
      pin = io_mcu_pin_table[i];
      if( pin.port != 0xff ){
         handle.port = pin.port;
         mcu_pio_get(&handle, &tmp);
         if( tmp & 1<<pin.pin ){
            result |= 1<<i;
         }
      }
   }
   *o_pinmask = result;
   return 0;
}
