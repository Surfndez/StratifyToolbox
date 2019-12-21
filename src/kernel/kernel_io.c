

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
   u8 port;
   u8 pin;
   u8 init_state;
   u8 resd; //alignment
   u32 init_flags;
} kernel_io_pin_t;

const kernel_io_pin_t kernel_io_pin_table[kernel_io_total] = {
   { PCAL6116A_PORTA, 4+8, 0, 0, PIO_FLAG_SET_OUTPUT }, //kernel_io_wifi_reset
   { PCAL6116A_PORTA, 5+8, 1, 0, PIO_FLAG_SET_OUTPUT }, //kernel_io_wifi_chip_enable
   { PCAL6116A_PORTA, 6+8, 1, 0, PIO_FLAG_SET_OUTPUT }, //kernel_io_wifi_wake
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

const toolbox_pin_t pin_table[TOOLBOX_IO_PIN_COUNT] = {
   { .port = 1, .pin = 8, .slave_addr = 0x20, .slave_pin = 0x00 }, //IO1 RX
   { .port = 1, .pin = 9, .slave_addr = 0x20, .slave_pin = 0x01 }, //IO2 TX
   { .port = 4, .pin = 11, .slave_addr = 0x20, .slave_pin = 0x02 }, //IO3 CS
   { .port = 4, .pin = 12, .slave_addr = 0x20, .slave_pin = 0x03 }, //IO4 SCK
   { .port = 2, .pin = 6, .slave_addr = 0x20, .slave_pin = 0x04 }, //IO5
   { .port = 2, .pin = 7, .slave_addr = 0x20, .slave_pin = 0x05 }, //IO6
   { .port = 0, .pin = 2, .slave_addr = 0x20, .slave_pin = 0x06 }, //IO8
   { .port = 0, .pin = 3, .slave_addr = 0x20, .slave_pin = 0x07 }, //IO9
   { .port = 0, .pin = 6, .slave_addr = 0x20, .slave_pin = 0x10 }, //IO10
   { .port = 0, .pin = 7, .slave_addr = 0x20, .slave_pin = 0x11 }, //IO11
   { .port = 0, .pin = 15, .slave_addr = 0x20, .slave_pin = 0x12 }, //IO12
   { .port = 0, .pin = 10, .slave_addr = 0x20, .slave_pin = 0x13 } //IO13
};

const toolbox_pin_t alt_pin_table[TOOLBOX_IO_PIN_COUNT] = {
   { .port = 2, .pin = 8, .slave_addr = 0x00, .slave_pin = 0x00 }, //IO1 SCL -- direct connection to pullups
   { .port = 0, .pin = 1, .slave_addr = 0x00, .slave_pin = 0x00 }, //IO2 SDA
   { .port = 1, .pin = 10, .slave_addr = 0x00, .slave_pin = 0x00 }, //IO3 SCL
   { .port = 1, .pin = 11, .slave_addr = 0x00, .slave_pin = 0x00 }, //IO4 SDA
   { .port = 4, .pin = 13, .slave_addr = 0x20, .slave_pin = 0x04 }, //IO5
   { .port = 4, .pin = 14, .slave_addr = 0x20, .slave_pin = 0x05 }, //IO6
   { .port = 1, .pin = 5, .slave_addr = 0x20, .slave_pin = 0x06 }, //IO8
   { .port = 4, .pin = 3, .slave_addr = 0x20, .slave_pin = 0x07 }, //IO9
   { .port = 4, .pin = 4, .slave_addr = 0x20, .slave_pin = 0x10 }, //IO10
   { .port = 4, .pin = 5, .slave_addr = 0x20, .slave_pin = 0x11 }, //IO11
   { .port = 4, .pin = 6, .slave_addr = 0x20, .slave_pin = 0x12 }, //IO12
   { .port = 0, .pin = 10, .slave_addr = 0x20, .slave_pin = 0x13 } //IO13 -- special case
};

const toolbox_pin_t swd_pin_table[TOOLBOX_SWD_PIN_COUNT] = {
   //SWD
   { .port = 0, .pin = 9, .slave_addr = 0x21, .slave_pin = 0x05 }, //SWDIO_TMS
   { .port = 1, .pin = 13, .slave_addr = 0x21, .slave_pin = 0x00 }, //SWDIO_TCK
   { .port = 3, .pin = 2, .slave_addr = 0x21, .slave_pin = 0x01 }, //SWDIO_TDO
   { .port = 3, .pin = 3, .slave_addr = 0x21, .slave_pin = 0x02 }, //SWDIO_TDI
   { .port = 3, .pin = 6, .slave_addr = 0x21, .slave_pin = 0x03 }, //SWDIO_RESET
   { .port = 4, .pin = 0, .slave_addr = 0x21, .slave_pin = 0x04 } //RTCK
};

int kernel_io_init(){
   if( pcal6416a_init() < 0 ){
      mcu_debug_log_error(MCU_DEBUG_USER0, "Failed to initialize GPIO expander");
      return -1;
   }

   for(u32 i=0; i < kernel_io_total; i++){
      int result;
      const kernel_io_pin_t * io_pin = kernel_io_pin_table + i;
      pio_attr_t attributes;
      if( io_pin->init_state ){
         kernel_io_set(i);
      } else {
         kernel_io_clear(i);
      }

      attributes.o_flags = io_pin->init_flags;
      attributes.o_pinmask = 1<<io_pin->pin;
      MCU_DEBUG_LINE_TRACE();

      result = pcal6416a_setattr(
               io_pin->port,
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

   MCU_DEBUG_LINE_TRACE();
   return 0;
}

int kernel_io_set(enum kernel_io_pins pin){
   const kernel_io_pin_t * io_pin = kernel_io_pin_table + pin;
   return pcal6416a_setmask(
            io_pin->port,
            1<<io_pin->pin
            );
}

int kernel_io_clear(enum kernel_io_pins pin){
   const kernel_io_pin_t * io_pin = kernel_io_pin_table + pin;
   return pcal6416a_clrmask(
            io_pin->port,
            1<<io_pin->pin
            );
}

int kernel_io_request(
      void * args
      ){
   const toolbox_io_attr_t * attributes = args;




   return 0;
}
