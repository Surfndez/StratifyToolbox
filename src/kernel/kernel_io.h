#ifndef KERNEL_REQUEST_IO_GATE_H
#define KERNEL_REQUEST_IO_GATE_H

#include <sos/sos.h>

#include <ToolboxAPI/toolbox_io.h>
#include "kernel_shared.h"


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
   kernel_io_first = kernel_io_wifi_reset,
   kernel_io_last = kernel_io_vdd_out_enable
};


int kernel_io_init();
int kernel_io_set(enum kernel_io_pins pin);
int kernel_io_clear(enum kernel_io_pins pin);

int kernel_io_is_direction_assigned(
      enum kernel_shared_direction_channels channel,
      u8 peripheral_function,
      u32 io_flags
      );

int kernel_io_request(
      void * args
      );


#endif // KERNEL_REQUEST_IO_GATE_H
