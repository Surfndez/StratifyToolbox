
#include <errno.h>
#include <mcu/core.h>
#include "kernel_shared.h"
#include "kernel_io.h"
#include "pcal6416a.h"




#define IN PIO_FLAG_SET_INPUT
#define OUT PIO_FLAG_SET_OUTPUT

#if defined __debug
#define DEBUG_UART_DIRECTION OUT
#else
#define DEBUG_UART_DIRECTION IN
#endif

typedef struct {
   u8 port;
   u8 pin;
   u8 slave_addr;
   u8 slave_pin;
} toolbox_pin_t;

typedef struct MCU_PACK {
   u8 expansion_port;
   u8 expansion_pin;
   u8 init_state;
   u8 resd; //alignment
   u32 init_flags;
} kernel_io_pin_t;

const kernel_io_pin_t internal_pin_table[kernel_io_total] = {
   { PCAL6116A_PORTA, 4+8, 0, 0, OUT }, //kernel_io_wifi_reset
   { PCAL6116A_PORTA, 5+8, 0, 0, OUT }, //kernel_io_wifi_chip_enable
   { PCAL6116A_PORTA, 6+8, 0, 0, OUT }, //kernel_io_wifi_wake
   { PCAL6116A_PORTB, 4+8, 0, 0, OUT }, //kernel_io_power_enable
   { PCAL6116A_PORTB, 3+8, 0, 0, OUT }, //kernel_io_vbat_measure_enable
   { PCAL6116A_PORTB, 2+8, 0, 0, IN | PIO_FLAG_IS_FLOAT }, //kernel_io_charge_fast_enable
   { PCAL6116A_PORTB, 1+8, 0, 0, IN | PIO_FLAG_IS_PULLUP }, //kernel_io_charge_stat1
   { PCAL6116A_PORTB, 0+8, 0, 0, IN | PIO_FLAG_IS_PULLUP }, //kernel_io_charge_stat2
   { PCAL6116A_PORTB, 6+0, 0, 0, IN | PIO_FLAG_IS_FLOAT }, //kernel_io_charge_proximity_mode
   { PCAL6116A_PORTB, 5+8, 0, 0, OUT }, //kernel_io_charge_vref_enable
   { PCAL6116A_PORTB, 6+8, 1, 0, OUT }, //kernel_io_charge_div10_enable
   { PCAL6116A_PORTB, 7+8, 1, 0, OUT }, //kernel_io_charge_vdd_out_enable
};

#define SPI(port) {CORE_PERIPH_SPI, port}
#define UART(port) {CORE_PERIPH_UART, port}
#define I2C(port) {CORE_PERIPH_I2C, port}
#define I2S(port) {CORE_PERIPH_I2S, port}
#define PIO(port) {CORE_PERIPH_PIO, port}
#define TMR(port) {CORE_PERIPH_TMR, port}
#define NONE(port) {CORE_PERIPH_RESERVED, port}

typedef struct MCU_PACK {
   u8 peripheral;
   u8 port;
} kernel_io_core_peripheral_t;



typedef struct MCU_PACK {
   u8 port;
   u8 pin;
   u8 expansion_port;
   u8 expansion_pin;
   u8 init_state;
   u8 resd; //alignment
   u8 init_flags;
   u8 primary_flags;
   u8 secondary_flags;
   kernel_io_core_peripheral_t core_peripheral[2]; //peripheral functions
} kernel_io_external_pin_t;

#if 0
static const kernel_io_external_pin_t external_pin_table[kernel_io_external_total] = {
   { 2, 8, PCAL6116A_PORTX, 0x00, 0, 0, IN, IN, 0, {TMR(3), NONE(0)} }, //IO1 SCL -- direct connection to pullups
   { 1, 8, PCAL6116A_PORTA, 0x00, 0, 0, IN, 0, IN, {I2C(1), UART(4)} }, //IO1 RX
   { 0, 1, PCAL6116A_PORTX, 0x00, 0, 0, IN, 0, IN, {NONE(0), TMR(2)} }, //IO2 SDA
   { 1, 9, PCAL6116A_PORTA, 0x01, 0, 0, DEBUG_UART_DIRECTION, 0, OUT, {I2C(1), UART(4)} }, //IO2 TX -- debugging output
   { 4, 11, PCAL6116A_PORTA, 0x02, 0, 0, IN, OUT, IN, {SPI(4), TMR(1)} }, //IO3 CS
   { 1, 10, PCAL6116A_PORTX, 0x00, 0, 0, IN, 0, 0, {I2C(2), TMR(2)} }, //IO3 SCL
   { 4, 12, PCAL6116A_PORTA, 0x03, 0, 0, IN, OUT, 0, {SPI(4), NONE(0)} }, //IO4 SCK
   { 1, 11, PCAL6116A_PORTX, 0x00, 0, 0, IN, 0, 0, {I2C(1), TMR(2)} }, //IO4 SDA
   { 4, 13, PCAL6116A_PORTA, 0x04, 0, 0, IN, IN, IN, {SPI(4), TMR(1)} }, //IO5 MISO
   { 2, 6, PCAL6116A_PORTA, 0x04, 0, 0, IN, OUT, IN, {UART(6), TMR(3)} }, //IO5
   { 4, 14, PCAL6116A_PORTA, 0x05, 0, 0, IN, OUT, IN, {SPI(4), TMR(1)} }, //IO6 MOSI
   { 2, 7, PCAL6116A_PORTA, 0x05, 0, 0, IN, IN, IN, {UART(6), TMR(3)} }, //IO6
   { 0xff, 0xff, PCAL6116A_PORTX, 0xff, 0xff, 0xff, 0, 0, 0, {NONE(0), NONE(0)} }, //IO7
   { 0xff, 0xff, PCAL6116A_PORTX, 0xff, 0xff, 0xff, 0, 0, 0, {NONE(0), NONE(0)} }, //IO7
   { 0, 2, PCAL6116A_PORTA, 0x06, 0, 0, IN, OUT, IN, {UART(2), TMR(2)} }, //IO8 TX
   { 1, 5, PCAL6116A_PORTA, 0x06, 0, 0, IN, 0, IN, {PIO(1), TMR(3)} }, //IO8 PIO
   { 0, 3, PCAL6116A_PORTA, 0x07, 0, 0, IN, IN, IN, {UART(2), TMR(2)} }, //IO9
   { 4, 3, PCAL6116A_PORTA, 0x07, 0, 0, IN, IN, 0, {I2S(4), NONE(0)} }, //IO9
   { 0, 6, PCAL6116A_PORTA, 0+8, 0, 0, IN, 0, IN, {PIO(0), TMR(3)} }, //IO10
   { 4, 4, PCAL6116A_PORTA, 0+8, 0, 0, IN, OUT, 0, {I2S(4), NONE(0)} }, //IO10
   { 0, 7, PCAL6116A_PORTA, 1+8, 0, 0, IN, 0, IN, {PIO(0), TMR(3)} }, //IO11
   { 4, 5, PCAL6116A_PORTA, 1+8, 0, 0, IN, OUT, 0, {I2S(4), NONE(0)} }, //IO11
   { 0, 15, PCAL6116A_PORTA, 2+8, 0, 0, IN, 0, IN, {PIO(1), TMR(2)} }, //IO12
   { 4, 6, PCAL6116A_PORTA, 2+8, 0, 0, IN, OUT, 0, {I2S(4), NONE(0)} }, //IO12
   { 0, 10, PCAL6116A_PORTA, 3+8, 0, 0, IN, IN, IN, {UART(1), TMR(1)} }, //IO13
   { 0, 9, PCAL6116A_PORTB, 5, 0, 0, OUT, OUT, IN, {UART(1), TMR(1)} }, //SWDIO_TMS --SWDIO
   { 1, 13, PCAL6116A_PORTB, 0, 0, 0, OUT, OUT, 0, {PIO(1), NONE(0)} }, //SWDIO_TCK
   { 3, 2, PCAL6116A_PORTB, 1, 0, 0, OUT, OUT, IN, {PIO(3), UART(5)} }, //SWDIO_TDO (UART5-RX)
   { 3, 3, PCAL6116A_PORTB, 2, 0, 0, IN, IN, IN, {PIO(3), TMR(1)} }, //SWDIO_TDI
   { 3, 6, PCAL6116A_PORTB, 3, 0, 0, OUT, OUT, IN, {PIO(3), TMR(3)} }, //SWDIO_RESET
   { 4, 0, PCAL6116A_PORTB, 4, 0, 0, OUT, IN, IN, {PIO(4), UART(4)} } //RTCK -- will probably not be available
};
#endif

static int init_internal_pins();
static int init_external_pins();
static int set_pin_direction(u32 pin_number, int io_flags);
static int set_io_function(const toolbox_io_attr_t * attributes);
static u32 decode_io_flags(u32 pin_number, u32 attribute_io_flags);

int kernel_io_request(
      void * args
      ){
   const toolbox_io_attr_t * attributes = args;

   if( attributes->o_flags & TOOLBOX_IO_GATE_FLAG_SET_IO_FUNCTION ){
      if( set_io_function(attributes) < 0 ){
         return -1;
      }
   } else if( attributes->o_flags & TOOLBOX_IO_GATE_FLAG_SET_SWD_FUNCTION ){

   } else if( attributes->o_flags & TOOLBOX_IO_GATE_FLAG_SET_SWD_FUNCTION ){

   }

   if( attributes->o_flags & TOOLBOX_IO_GATE_FLAG_ENABLE_DIV10_OUT ){
      kernel_io_clear(kernel_io_vref_enable); //active low
   } else if( attributes->o_flags & TOOLBOX_IO_GATE_FLAG_DISABLE_DIV10_OUT ){
      kernel_io_set(kernel_io_vref_enable); //active low
   }

   if( attributes->o_flags & TOOLBOX_IO_GATE_FLAG_ENABLE_VREF ){
      kernel_io_set(kernel_io_vref_enable); //active high
   } else if( attributes->o_flags & TOOLBOX_IO_GATE_FLAG_DISABLE_VREF ){
      kernel_io_clear(kernel_io_vref_enable);
   }

   if( attributes->o_flags & TOOLBOX_IO_GATE_FLAG_ENABLE_VDD_OUT ){
      kernel_io_clear(kernel_io_vdd_out_enable); //active low
   } else if( attributes->o_flags & TOOLBOX_IO_GATE_FLAG_DISABLE_VDD_OUT ){
      kernel_io_set(kernel_io_vdd_out_enable);
   }

   return 0;
}

int kernel_io_is_peripheral_pin_assigned_to_peripheral(
      mcu_pin_t mcu_pin,
      u8 core_peripheral
      ){
   //here mcu_pin is the actual mcu_pin
   //lookup in the pin number in the table -- then see if kernel_shared allows the peripheral
   for(u32 i=0; i < kernel_io_external_total; i++){
      if( (mcu_pin.port == external_pin_table[i].port) &&
          (mcu_pin.pin == external_pin_table[i].pin)
          ){
         if( kernel_shared_get_pin_state(i)->peripheral_function == core_peripheral ){
            return kernel_shared_get_pin_state(i)->io_flags;
         } else {
            return SYSFS_SET_RETURN(EINVAL);
         }
      }
   }

   return SYSFS_SET_RETURN(EINVAL);
}

int kernel_io_is_pin_assigned_to_pio(
      mcu_pin_t pio_device_pin,
      u32 io_flags
      ){

   mcu_pin_t actual_mcu_pin;

   if( (pio_device_pin.port == TOOLBOX_IO_PORT) &&
       (pio_device_pin.pin < TOOLBOX_IO_PIN_COUNT) ){
      u8 pin_number = pio_device_pin.pin*2;
      actual_mcu_pin.port = external_pin_table[pin_number].port;
      actual_mcu_pin.pin = external_pin_table[pin_number].pin;

   } else if( (pio_device_pin.port == TOOLBOX_IO_SWD_PORT) &&
              (pio_device_pin.pin < TOOLBOX_IO_SWD_PIN_COUNT) ){
      u8 pin_number = pio_device_pin.pin + kernel_io13_primary + 1;
      actual_mcu_pin.port = external_pin_table[pin_number].port;
      actual_mcu_pin.pin = external_pin_table[pin_number].pin;
   } else {
      return SYSFS_SET_RETURN(EINVAL);
   }

   int active_flags = kernel_io_is_peripheral_pin_assigned_to_peripheral(
            actual_mcu_pin,
            CORE_PERIPH_PIO
            );

   if( (io_flags & IN) && (active_flags & OUT) ){
      return SYSFS_SET_RETURN(EPROTO);
   }

   if( (io_flags & OUT) && (active_flags & IN) ){
      return SYSFS_SET_RETURN(EPROTO);
   }

   return active_flags;
}

int kernel_io_init(){
   if( pcal6416a_init() < 0 ){
      mcu_debug_log_error(MCU_DEBUG_USER0, "Failed to initialize GPIO expander");
      return -1;
   }

   if( init_internal_pins() < 0 ){
      return -1;
   }

   if( init_external_pins() < 0 ){
      return -1;
   }

   MCU_DEBUG_LINE_TRACE();
   return 0;
}

int kernel_io_set(enum kernel_io_pins pin){
   const kernel_io_pin_t * io_pin = internal_pin_table + pin;
   return pcal6416a_setmask(
            io_pin->expansion_port,
            1<<io_pin->expansion_pin
            );
}

int kernel_io_clear(enum kernel_io_pins pin){
   const kernel_io_pin_t * io_pin = internal_pin_table + pin;
   return pcal6416a_clrmask(
            io_pin->expansion_port,
            1<<io_pin->expansion_pin
            );
}

typedef struct {
   const toolbox_io_attr_t * attributes;
   int result;
   u8 pin_numbers[TOOLBOX_IO_PIN_ASSIGNMENT_CHANNEL_COUNT*2];
} svcall_acquire_pin_t;

void svcall_aquire_pin(void * args){
   CORTEXM_SVCALL_ENTER();
   svcall_acquire_pin_t * p = args;
   const toolbox_io_attr_t * attributes = p->attributes;

   for(u32 i=0; i < TOOLBOX_IO_PIN_ASSIGNMENT_CHANNEL_COUNT; i++){

      //caller uses these to set the IO gate values
      p->pin_numbers[i*2] = 0;
      p->pin_numbers[i*2+1] = 0;

      //what peripheral is being requested
      u8 core_peripheral = attributes->peripheral_function;
      //is peripheral available on the pin
      mcu_pin_t pin = attributes->pin_assignment.channel[i];
      u8 primary_pin_number = 0;
      u8 secondary_pin_number = 0;
      if(( pin.port == TOOLBOX_IO_PORT) && (pin.pin < TOOLBOX_IO_PIN_COUNT) ){
         primary_pin_number = pin.pin*2;
         if( pin.pin*2 != kernel_io13_primary ){
            secondary_pin_number = primary_pin_number + 1;
         } else {
            secondary_pin_number = primary_pin_number;
         }
      } else if( (pin.port == TOOLBOX_IO_SWD_PORT) && (pin.pin < TOOLBOX_IO_SWD_PIN_COUNT) ){
         primary_pin_number = pin.pin + kernel_io13_primary + 1;
         secondary_pin_number = primary_pin_number;
      } else {
         p->result = SYSFS_SET_RETURN(EINVAL);
         return;
      }

      const kernel_io_external_pin_t * primary_pin = external_pin_table + primary_pin_number;
      const kernel_io_external_pin_t * secondary_pin = external_pin_table + secondary_pin_number;

      /* 1 for primary 1
       * 2 for primary 2
       * 3 for secondary 1
       * 4 for secondary 2
       */
      u8 function_selection = 0;

      if( core_peripheral != CORE_PERIPH_RESERVED ){
         if( primary_pin->core_peripheral[0].peripheral == core_peripheral ){
            function_selection = 1;
         } else if( primary_pin->core_peripheral[1].peripheral == core_peripheral ){
            function_selection = 2;
         } else if( secondary_pin->core_peripheral[0].peripheral == core_peripheral ){
            function_selection = 3;
         } else if( secondary_pin->core_peripheral[1].peripheral == core_peripheral ){
            function_selection = 4;
         }

         if( function_selection != 0 ){
            //this is valid -- is the pin available or is it busy?
            if( kernel_shared_get_pin_state(primary_pin_number)->peripheral_function
                == CORE_PERIPH_RESERVED ){
               //both are aquired to the
               u32 primary_flags;
               u32 secondary_flags;
               switch(function_selection){
                  case 1:
                     primary_flags = primary_pin->primary_flags;
                     secondary_flags = secondary_pin->init_flags;
                     break;
                  case 2:
                     primary_flags = primary_pin->secondary_flags;
                     secondary_flags = secondary_pin->init_flags;
                     break;
                  case 3:
                     primary_flags = primary_pin->init_flags;
                     secondary_flags = secondary_pin->primary_flags;
                     break;
                  case 4:
                     primary_flags = primary_pin->init_flags;
                     secondary_flags = secondary_pin->secondary_flags;
                     break;
               }

               if( function_selection < 3 ){
                  //set secondary first in case pins are the same
                  kernel_shared_root_set_pin_state(
                           secondary_pin_number,
                           core_peripheral,
                           secondary_flags
                           );

                  kernel_shared_root_set_pin_state(
                           primary_pin_number,
                           core_peripheral,
                           primary_flags
                           );

               } else {
                  //set primary first in case pins are the same
                  kernel_shared_root_set_pin_state(
                           primary_pin_number,
                           core_peripheral,
                           primary_flags
                           );

                  kernel_shared_root_set_pin_state(
                           secondary_pin_number,
                           core_peripheral,
                           secondary_flags
                           );
               }

               //tell the caller which pins were updated
               p->pin_numbers[i*2] = primary_pin_number;
               p->pin_numbers[i*2+1] = secondary_pin_number;
            } else {
               p->result = SYSFS_SET_RETURN(EINVAL);
               return;
            }
         } else {
            //not a valid request
            p->result = SYSFS_SET_RETURN(EINVAL);
            return;
         }
      }
   }
}

int set_io_function(const toolbox_io_attr_t * attributes){

   //check kernel if pins are busy on a different peripheral

   if( attributes->peripheral_function == CORE_PERIPH_I2C ){

      switch(attributes->peripheral_port){
         case 0:
            //I2C1
            set_pin_direction(kernel_io1_secondary, IN);
            set_pin_direction(kernel_io2_secondary, IN);
            break;

         case 1:
            //I2C2
            //get pins from the kernel

            //set pins as input
            set_pin_direction(kernel_io3_primary, IN);
            set_pin_direction(kernel_io4_primary, IN);
            break;
      }

   }

   if( attributes->peripheral_function == CORE_PERIPH_SPI ){
      if( attributes->o_flags & TOOLBOX_IO_IS_INVERT_DIRECTION ){
         //slave
         set_pin_direction(kernel_io3_primary, IN);
         set_pin_direction(kernel_io4_primary, IN);
         set_pin_direction(kernel_io5_primary, OUT);
         set_pin_direction(kernel_io6_primary, IN);
      } else {
         //master
         set_pin_direction(kernel_io3_primary, OUT);
         set_pin_direction(kernel_io4_primary, OUT);
         set_pin_direction(kernel_io5_primary, IN);
         set_pin_direction(kernel_io6_primary, OUT);
      }
   }

   if( attributes->peripheral_function == CORE_PERIPH_I2S ){
      if( attributes->o_flags & TOOLBOX_IO_IS_INVERT_DIRECTION ){
         //slave
         set_pin_direction(kernel_io9_primary, OUT);
         set_pin_direction(kernel_io10_primary, IN);
         set_pin_direction(kernel_io11_primary, IN);
         set_pin_direction(kernel_io12_primary, IN);
      } else {
         //master
         set_pin_direction(kernel_io9_primary, IN);
         set_pin_direction(kernel_io10_primary, OUT);
         set_pin_direction(kernel_io11_primary, OUT);
         set_pin_direction(kernel_io12_primary, OUT);
      }
   }

   if( attributes->peripheral_function == CORE_PERIPH_UART ){
      switch(attributes->peripheral_port){
         case 0:
            set_pin_direction(kernel_io13_primary, IN); //sole pin
            break;

         case 1:
            set_pin_direction(kernel_io8_primary, OUT);
            set_pin_direction(kernel_io3_primary, IN);
            break;

         case 3:
            set_pin_direction(kernel_io1_primary, IN);
            set_pin_direction(kernel_io2_primary, OUT);
            break;

         case 5:
            set_pin_direction(kernel_io5_primary, OUT);
            set_pin_direction(kernel_io6_primary, IN);
            break;
      }
   }

   if( attributes->peripheral_function == CORE_PERIPH_PIO ){
      if( (attributes->peripheral_port == 0) &&
          (attributes->pin_number < TOOLBOX_IO_PIN_COUNT) ){
         if( attributes->o_flags & TOOLBOX_IO_IS_OUTPUT ){
            set_pin_direction(attributes->pin_number*2, OUT);
         } else {
            set_pin_direction(attributes->pin_number*2, IN);
         }
      }
   }




   svcall_acquire_pin_t args;
   args.attributes = attributes;
   args.result = 0;
   cortexm_svcall(svcall_aquire_pin, &args);
   if( args.result < 0 ){
      errno = SYSFS_GET_RETURN_ERRNO(args.result);
      return SYSFS_GET_RETURN(args.result);
   }

   //now configure the IO based on the seleted pins
   for(u32 i=0; i < TOOLBOX_IO_PIN_ASSIGNMENT_CHANNEL_COUNT; i++){
      u8 primary_pin_number = args.pin_numbers[i*2];
      u8 secondary_pin_number = args.pin_numbers[i*2+1];
      u32 io_flags = decode_io_flags(primary_pin_number, attributes->o_flags);
      mcu_debug_log_info(
               MCU_DEBUG_USER0,
               "set primary pin direction %d -> %d",
               primary_pin_number,
               io_flags
               );
      set_pin_direction(
               primary_pin_number,
               io_flags
               );

      if( secondary_pin_number != primary_pin_number ){
         io_flags = decode_io_flags(secondary_pin_number, attributes->o_flags);

         mcu_debug_log_info(
                  MCU_DEBUG_USER0,
                  "set secondary pin direction %d -> %d",
                  secondary_pin_number,
                  io_flags
                  );

         set_pin_direction(
                  secondary_pin_number,
                  decode_io_flags(secondary_pin_number, attributes->o_flags)
                  );
      }
   }

   return 0;
}

u32 decode_io_flags(u32 pin_number, u32 attribute_io_flags){
   u32 io_flags = kernel_shared_get_pin_state(pin_number)->io_flags;
   if( io_flags == 0 ){
      if( attribute_io_flags & TOOLBOX_IO_IS_OUTPUT ){
         io_flags = OUT;
      } else {
         io_flags = IN;
      }
   }

   u8 peripheral_function = kernel_shared_get_pin_state(pin_number)->peripheral_function;
   if( (peripheral_function == CORE_PERIPH_SPI) ||
       (peripheral_function == CORE_PERIPH_I2S)
       ){
      if( attribute_io_flags & TOOLBOX_IO_IS_INVERT_DIRECTION ){
         if( io_flags == IN ){
            io_flags = OUT;
         } else {
            io_flags = IN;
         }
      }
   }
   return io_flags;
}

int set_pin_direction(u32 pin_number, int io_flags){
   u8 port = external_pin_table[pin_number].expansion_port;
   u32 o_pinmask = 1<<external_pin_table[pin_number].expansion_pin;
   if( io_flags == IN ){
      pcal6416a_setmask(
               port,
               o_pinmask
               );
   } else {
      pcal6416a_clrmask(
               port,
               o_pinmask
               );
   }
   return 0;
}

int init_external_pins(){
   for(u32 i=0; i < kernel_io_external_total; i++){
      int result;
      const kernel_io_external_pin_t * io_pin = external_pin_table + i;
      pio_attr_t attributes;
      if( io_pin->init_state ){
         kernel_io_set(i);
      } else {
         kernel_io_clear(i);
      }

      attributes.o_flags = io_pin->init_flags;
      attributes.o_pinmask = 1<<io_pin->expansion_pin;
      MCU_DEBUG_LINE_TRACE();

      result = pcal6416a_setattr(
               io_pin->expansion_port,
               &attributes
               );
      if( result < 0 ){
         mcu_debug_log_error(
                  MCU_DEBUG_USER0,
                  "failed to set pin attributes",
                  result
                  );
         return -1;
      }
   }
   return 0;
}

int init_internal_pins(){
   for(u32 i=0; i < kernel_io_total; i++){
      int result;
      const kernel_io_pin_t * io_pin = internal_pin_table + i;
      pio_attr_t attributes;
      if( io_pin->init_state ){
         kernel_io_set(i);
      } else {
         kernel_io_clear(i);
      }

      attributes.o_flags = io_pin->init_flags;
      attributes.o_pinmask = 1<<io_pin->expansion_pin;
      MCU_DEBUG_LINE_TRACE();

      result = pcal6416a_setattr(
               io_pin->expansion_port,
               &attributes
               );
      if( result < 0 ){
         mcu_debug_log_error(
                  MCU_DEBUG_USER0,
                  "failed to set pin attributes",
                  result
                  );
         return -1;
      }
   }
   return 0;
}


