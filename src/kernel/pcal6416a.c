#include <fcntl.h>
#include <mcu/debug.h>
#include <sos/dev/i2c.h>
#include <sos/fs/sysfs.h>

#include "pcal6416a.h"
#include "kernel_shared.h"


enum register_commands {
   command_input = 0x0,
   command_output = 0x2,
   command_polarity_inversion = 0x4,
   command_configuration = 0x6,
   command_output_drive0 = 0x40,
   command_output_drive1 = 0x42,
   command_input_latch = 0x44,
   command_pullup_pulldown_enable = 0x46,
   command_pullup_pulldown_selection = 0x48,
   command_interrupt_mask = 0x4a,
   command_interrupt_status = 0x4c,
   command_output_port_configuration = 0x4f //only 1 byte -- open drain or push-pull
};


static int lock_i2c(u8 slave_address);
static int unlock_i2c();
static u16 set_register_bits(u16 value, u16 o_mask){
   return value | o_mask;
}
static u16 clear_register_bits(u16 value, u16 o_mask){
   return value & ~o_mask;
}
static int update_register(
      enum register_commands command,
      u16 o_mask,
      u16 (*update_function)(u16, u16)
      );


int pcal6416a_init(){
   return 0;
}

int pcal6416a_setattr(u8 slave_address, const pio_attr_t * attributes){
   const u32 o_flags = attributes->o_flags;
   const u32 o_pinmask = attributes->o_pinmask;

   if( lock_i2c(slave_address) < 0 ){
      mcu_debug_log_error(MCU_DEBUG_USER0, "%s():%d", __FUNCTION__, __LINE__);
      return -1;
   }

   if( o_flags & PIO_FLAG_SET_OUTPUT ){
      //update configuration register
      //mcu_debug_printf("configure 0x%X:0x%X as output\n", slave_address, o_pinmask);
      if( update_register(command_configuration, o_pinmask, clear_register_bits) < 0 ){
         mcu_debug_log_error(MCU_DEBUG_USER0, "%s():%d", __FUNCTION__, __LINE__);
      }
   } else if( o_flags & PIO_FLAG_SET_INPUT ){
      //mcu_debug_printf("configure 0x%X:0x%X as input\n", slave_address, o_pinmask);
      update_register(command_configuration, o_pinmask, set_register_bits);

      if( (o_flags & PIO_FLAG_IS_PULLDOWN) ||
          (o_flags & PIO_FLAG_IS_PULLUP) ){
         update_register(
                  command_pullup_pulldown_enable,
                  o_pinmask,
                  set_register_bits
                  );

         update_register(
                  command_pullup_pulldown_selection,
                  o_pinmask,
                  (o_flags & PIO_FLAG_IS_PULLUP) ? set_register_bits : clear_register_bits
                                                   );
      } else {
         update_register(
                  command_pullup_pulldown_enable,
                  o_pinmask,
                  clear_register_bits
                  );
      }
   }

   unlock_i2c();

   return 0;
}

u16 pcal6416a_read(u8 slave_address){
   u16 value;

   if( lock_i2c(slave_address) < 0 ){
      return 0xffff;
   }

   return sysfs_file_read(
            kernel_shared_i2c_file(),
            &value,
            2
            );

   unlock_i2c();

   return value;

}

int pcal6416a_setmask(u8 slave_address, u16 o_mask){
   if( lock_i2c(slave_address) < 0 ){
      return -1;
   }
   int result = update_register(command_output, o_mask, set_register_bits);
   unlock_i2c();
   return result;
}

int pcal6416a_clrmask(u8 slave_address, u16 o_mask){
   if( lock_i2c(slave_address) < 0 ){
      mcu_debug_log_error(MCU_DEBUG_USER0, "%s():%d", __FUNCTION__, __LINE__);
      return -1;
   }
   int result = update_register(command_output, o_mask, clear_register_bits);
   unlock_i2c();
   return result;
}

int lock_i2c(u8 slave_address){
   int result;
   //lock mutex

   if( pthread_mutex_lock(
          kernel_shared_i2c_mutex()
          ) < 0 ){
      mcu_debug_log_error(
               MCU_DEBUG_USER0,
               "failed to lock mutex for 0x%X",
               slave_address
               );
      return -1;
   }

   i2c_attr_t attributes = {0};
   attributes.o_flags = I2C_FLAG_PREPARE_PTR_DATA;
   attributes.slave_addr[0].addr8[0] = slave_address;
   result = sysfs_file_ioctl(
            kernel_shared_i2c_file(),
            I_I2C_SETATTR,
            &attributes
            );
   if( result < 0 ){
      return -1;
   }

   return 0;
}

int unlock_i2c(){

   if( pthread_mutex_unlock(
          kernel_shared_i2c_mutex()
          ) < 0 ){
      return -1;
   }
   //unlock mutex
   return 0;
}


int update_register(
      enum register_commands command,
      u16 o_mask,
      u16 (*update_function)(u16, u16)
      ){

   u16 value;
   int result;

   kernel_shared_i2c_file()->loc = command;
   result = sysfs_file_read(
            kernel_shared_i2c_file(),
            &value,
            2
            );

   if( result == 2 ){
      value = update_function(value, o_mask);
      result = sysfs_file_write(
               kernel_shared_i2c_file(),
               &value,
               2
               );
      if( result != 2 ){
         mcu_debug_log_error(MCU_DEBUG_USER0, "%s():%d", __FUNCTION__, __LINE__);
      }
   } else {
      mcu_debug_log_error(
               MCU_DEBUG_USER0,
               "%s():%d",
               __FUNCTION__,
               __LINE__
               );
   }

   return result == 2 ? 0 : -2;
}


