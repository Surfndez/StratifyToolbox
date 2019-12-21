
#include <mcu/core.h>
#include "kernel_shared.h"
#include "kernel_io.h"
#include "pcal6416a.h"

#define TOOLBOX_IO_PIN_COUNT 12
#define TOOLBOX_SWD_PIN_COUNT 6
#define TOOLBOX_PIN_COUNT (TOOLBOX_IO_PIN_COUNT + TOOLBOX_SWD_PIN_COUNT)
#define TOOLBOX_IO_PORT_COUNT 3

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
   { PCAL6116A_PORTA, 4+8, 0, 0, PIO_FLAG_SET_OUTPUT }, //kernel_io_wifi_reset
   { PCAL6116A_PORTA, 5+8, 0, 0, PIO_FLAG_SET_OUTPUT }, //kernel_io_wifi_chip_enable
   { PCAL6116A_PORTA, 6+8, 0, 0, PIO_FLAG_SET_OUTPUT }, //kernel_io_wifi_wake
   { PCAL6116A_PORTB, 4+8, 0, 0, PIO_FLAG_SET_OUTPUT }, //kernel_io_power_enable
   { PCAL6116A_PORTB, 3+8, 0, 0, PIO_FLAG_SET_OUTPUT }, //kernel_io_vbat_measure_enable
   { PCAL6116A_PORTB, 2+8, 0, 0, PIO_FLAG_SET_INPUT | PIO_FLAG_IS_FLOAT }, //kernel_io_charge_fast_enable
   { PCAL6116A_PORTB, 1+8, 0, 0, PIO_FLAG_SET_INPUT | PIO_FLAG_IS_PULLUP }, //kernel_io_charge_stat1
   { PCAL6116A_PORTB, 0+8, 0, 0, PIO_FLAG_SET_INPUT | PIO_FLAG_IS_PULLUP }, //kernel_io_charge_stat2
   { PCAL6116A_PORTB, 6+0, 0, 0, PIO_FLAG_SET_INPUT | PIO_FLAG_IS_FLOAT }, //kernel_io_charge_proximity_mode
   { PCAL6116A_PORTB, 5+8, 0, 0, PIO_FLAG_SET_OUTPUT }, //kernel_io_charge_vref_enable
   { PCAL6116A_PORTB, 6+8, 1, 0, PIO_FLAG_SET_OUTPUT }, //kernel_io_charge_div10_enable
   { PCAL6116A_PORTB, 7+8, 1, 0, PIO_FLAG_SET_OUTPUT }, //kernel_io_charge_vdd_out_enable
};

#define SPI(port) CORE_PERIPH_SPI, port
#define UART(port) CORE_PERIPH_UART, port
#define I2C(port) CORE_PERIPH_I2C, port
#define I2S(port) CORE_PERIPH_I2S, port
#define PIO(port) CORE_PERIPH_PIO, port
#define TMR(port) CORE_PERIPH_TMR, port
#define NONE(port) 0xff, port

typedef struct MCU_PACK {
   u8 port;
   u8 pin;
   u8 expansion_port;
   u8 expansion_pin;
   u8 init_state;
   u8 resd; //alignment
   u32 init_flags;
   u8 core_peripheral[4]; //peripheral functions
} kernel_io_external_pin_t;

static const kernel_io_external_pin_t external_pin_table[kernel_io_external_total] = {
   { 2, 8, PCAL6116A_PORTX, 0x00, 0, 0, PIO_FLAG_SET_INPUT, {TMR(3), NONE(0)} }, //IO1 SCL -- direct connection to pullups
   { 1, 8, PCAL6116A_PORTA, 0x00, 0, 0, PIO_FLAG_SET_INPUT, {I2C(1), UART(4)} }, //IO1 RX
   { 0, 1, PCAL6116A_PORTX, 0x00, 0, 0, PIO_FLAG_SET_INPUT, {NONE(0), TMR(2)} }, //IO2 SDA
   { 1, 9, PCAL6116A_PORTA, 0x01, 0, 0, PIO_FLAG_SET_OUTPUT, {I2C(1), UART(4)} }, //IO2 TX -- debugging output
   { 4, 11, PCAL6116A_PORTA, 0x02, 0, 0, PIO_FLAG_SET_INPUT, {SPI(4), TMR(1)} }, //IO3 CS
   { 1, 10, PCAL6116A_PORTX, 0x00, 0, 0, PIO_FLAG_SET_INPUT, {I2C(2), TMR(2)} }, //IO3 SCL
   { 4, 12, PCAL6116A_PORTA, 0x03, 0, 0, PIO_FLAG_SET_INPUT, {SPI(4), NONE(0)} }, //IO4 SCK
   { 1, 11, PCAL6116A_PORTX, 0x00, 0, 0, PIO_FLAG_SET_INPUT, {I2C(1), TMR(2)} }, //IO4 SDA
   { 4, 13, PCAL6116A_PORTA, 0x04, 0, 0, PIO_FLAG_SET_INPUT, {SPI(4), TMR(1)} }, //IO5
   { 2, 6, PCAL6116A_PORTA, 0x04, 0, 0, PIO_FLAG_SET_INPUT, {UART(6), TMR(3)} }, //IO5
   { 4, 14, PCAL6116A_PORTA, 0x05, 0, 0, PIO_FLAG_SET_INPUT, {SPI(4), TMR(1)} }, //IO6
   { 2, 7, PCAL6116A_PORTA, 0x05, 0, 0, PIO_FLAG_SET_INPUT, {UART(6), TMR(3)} }, //IO6
   { 0xff, 0xff, PCAL6116A_PORTX, 0xff, 0xff, 0xff, 0, {NONE(0), NONE(0)} }, //IO7
   { 0xff, 0xff, PCAL6116A_PORTX, 0xff, 0xff, 0xff, 0, {NONE(0), NONE(0)} }, //IO7
   { 0, 2, PCAL6116A_PORTA, 0x06, 0, 0, PIO_FLAG_SET_INPUT, {UART(2), TMR(2)} }, //IO8
   { 1, 5, PCAL6116A_PORTA, 0x06, 0, 0, PIO_FLAG_SET_INPUT, {PIO(1), TMR(3)} }, //IO8
   { 0, 3, PCAL6116A_PORTA, 0x07, 0, 0, PIO_FLAG_SET_INPUT, {UART(2), TMR(2)} }, //IO9
   { 4, 3, PCAL6116A_PORTA, 0x07, 0, 0, PIO_FLAG_SET_INPUT, {I2S(4), NONE(0)} }, //IO9
   { 0, 6, PCAL6116A_PORTA, 0+8, 0, 0, PIO_FLAG_SET_INPUT, {PIO(0), TMR(3)} }, //IO10
   { 4, 4, PCAL6116A_PORTA, 0+8, 0, 0, PIO_FLAG_SET_INPUT, {I2S(4), NONE(0)} }, //IO10
   { 0, 7, PCAL6116A_PORTA, 1+8, 0, 0, PIO_FLAG_SET_INPUT, {PIO(0), TMR(3)} }, //IO11
   { 4, 5, PCAL6116A_PORTA, 1+8, 0, 0, PIO_FLAG_SET_INPUT, {I2S(4), NONE(0)} }, //IO11
   { 0, 15, PCAL6116A_PORTA, 2+8, 0, 0, PIO_FLAG_SET_INPUT, {PIO(1), TMR(2)} }, //IO12
   { 4, 6, PCAL6116A_PORTA, 2+8, 0, 0, PIO_FLAG_SET_INPUT, {I2S(4), NONE(0)} }, //IO12
   { 0, 10, PCAL6116A_PORTA, 3+8, 0, 0, PIO_FLAG_SET_INPUT, {PIO(0), TMR(1)} }, //IO13
   { 0, 9, PCAL6116A_PORTB, 5, 0, 0, PIO_FLAG_SET_OUTPUT, {PIO(0), NONE(1)} }, //SWDIO_TMS --SWDIO
   { 1, 13, PCAL6116A_PORTB, 0, 0, 0, PIO_FLAG_SET_OUTPUT, {PIO(1), TMR(1)} }, //SWDIO_TCK
   { 3, 2, PCAL6116A_PORTB, 1, 0, 0, PIO_FLAG_SET_OUTPUT, {PIO(3), UART(5)} }, //SWDIO_TDO (UART5-RX)
   { 3, 3, PCAL6116A_PORTB, 2, 0, 0, PIO_FLAG_SET_INPUT, {PIO(3), TMR(1)} }, //SWDIO_TDI
   { 3, 6, PCAL6116A_PORTB, 3, 0, 0, PIO_FLAG_SET_OUTPUT, {PIO(3), TMR(1)} }, //SWDIO_RESET
   { 4, 0, PCAL6116A_PORTB, 4, 0, 0, PIO_FLAG_SET_OUTPUT, {PIO(4), TMR(1)} } //RTCK
};

static int init_internal_pins();
static int init_external_pins();
static int set_io_function(const toolbox_io_attr_t * attributes);

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

void svcall_aquire_pin(void * args){
   const toolbox_io_attr_t * attributes = args;
}

int set_io_function(const toolbox_io_attr_t * attributes){
   if( attributes->peripheral_function == CORE_PERIPH_SPI ){
      //only one SPI available -- are all pins needed



   } else if( attributes->peripheral_function == CORE_PERIPH_UART ){

   } else if( attributes->peripheral_function == CORE_PERIPH_I2C ){

   } else if( attributes->peripheral_function == CORE_PERIPH_I2S ){

   } else if( attributes->peripheral_function == CORE_PERIPH_PIO ){

   } else if( attributes->peripheral_function == CORE_PERIPH_TMR ){

   }
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
