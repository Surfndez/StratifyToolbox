#include <fcntl.h>
#include <mcu/debug.h>
#include <sos/dev/i2c.h>

#include "pcal6416a.h"
#include "kernel_shared.h"
#include "i2c_internal.h"


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

   if( i2c_internal_lock(slave_address) < 0 ){
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

   i2c_internal_unlock();

   return 0;
}

u16 pcal6416a_read(u8 slave_address){
   u16 value;

   if( i2c_internal_lock(slave_address) < 0 ){
      return 0xffff;
   }

   return i2c_internal_read(
            command_input,
            &value,
            2
            );

   i2c_internal_unlock();

   return value;

}

int pcal6416a_setmask(u8 slave_address, u16 o_mask){
   if( i2c_internal_lock(slave_address) < 0 ){
      return -1;
   }
   int result = update_register(command_output, o_mask, set_register_bits);
   i2c_internal_unlock();
   return result;
}

int pcal6416a_clrmask(u8 slave_address, u16 o_mask){
   if( i2c_internal_lock(slave_address) < 0 ){
      mcu_debug_log_error(MCU_DEBUG_USER0, "%s():%d", __FUNCTION__, __LINE__);
      return -1;
   }
   int result = update_register(command_output, o_mask, clear_register_bits);
   i2c_internal_unlock();
   return result;
}

int update_register(
      enum register_commands command,
      u16 o_mask,
      u16 (*update_function)(u16, u16)
      ){

   u16 value;
   int result;

   result = i2c_internal_read(
            command,
            &value,
            2
            );

   if( result == 2 ){
      value = update_function(value, o_mask);
      result = i2c_internal_write(
               command,
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


