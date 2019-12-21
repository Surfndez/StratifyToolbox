#ifndef KERNEL_REQUEST_IO_GATE_H
#define KERNEL_REQUEST_IO_GATE_H

#include <sos/sos.h>


enum toolbox_io_flags {
   TOOLBOX_IO_GATE_FLAG_SET_IO_FUNCTION = (1<<0),
   TOOLBOX_IO_GATE_FLAG_SET_SWD_FUNCTION = (1<<1),
   TOOLBOX_IO_GATE_FLAG_SET_JTAG_FUNCTION = (1<<2),
   TOOLBOX_IO_GATE_FLAG_ENABLE_VREF = (1<<3),
   TOOLBOX_IO_GATE_FLAG_DISABLE_VREF = (1<<4),
   TOOLBOX_IO_GATE_FLAG_ENABLE_DIV10_OUT = (1<<5),
   TOOLBOX_IO_GATE_FLAG_DISABLE_DIV10_OUT = (1<<6),
   TOOLBOX_IO_GATE_FLAG_ENABLE_VDD_OUT = (1<<7),
   TOOLBOX_IO_GATE_FLAG_DISABLE_VDD_OUT = (1<<8),
};

typedef struct {
   mcu_pin_t channel[4]; //port is 0 for IO, 1 for JTAG
} toolbox_io_pin_assignment_t;

typedef struct {
   u32 o_flags;
   u8 peripheral_function[32]; //get current status of device
   u8 peripheral_port[32];
} toolbox_io_info_t;

typedef struct {
   u32 o_flags;
   toolbox_io_pin_assignment_t pin_assignment;
   u8 peripheral_function; //use CORE_PERIPH_UART, etc
   u8 peripheral_port;
} toolbox_io_attr_t;

enum kernel_io_pins {
   kernel_io_wifi_reset,
   kernel_io_wifi_chip_enable,
   kernel_io_wifi_wake,
   kernel_io_power_enable,
   kernel_io_vbat_measure_enable,
   kernel_io_charge_fast_enable,
   kernel_io_charge_stat1,
   kernel_io_charge_stat2,
   kernel_io_proximity_mode,
   kernel_io_vref_enable,
   kernel_io_div10_enable,
   kernel_io_vdd_out_enable,
   kernel_io_total
};


int kernel_io_init();
int kernel_io_set(enum kernel_io_pins pin);
int kernel_io_clear(enum kernel_io_pins pin);

int kernel_io_request(
      void * args
      );


#endif // KERNEL_REQUEST_IO_GATE_H
