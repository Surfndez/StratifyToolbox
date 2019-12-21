#ifndef KERNEL_SHARED_H
#define KERNEL_SHARED_H

#include <sos/fs/sysfs.h>
#include <pthread.h>

#if 0
enum kernel_io_external_pins {
   kernel_io1_primary,
   kernel_io1_secondary,
   kernel_io2_primary,
   kernel_io2_secondary,
   kernel_io3_primary,
   kernel_io3_secondary,
   kernel_io4_primary,
   kernel_io4_secondary,
   kernel_io5_primary,
   kernel_io5_secondary,
   kernel_io6_primary,
   kernel_io6_secondary,
   kernel_io7_primary,
   kernel_io7_secondary,
   kernel_io8_primary,
   kernel_io8_secondary,
   kernel_io9_primary,
   kernel_io9_secondary,
   kernel_io10_primary,
   kernel_io10_secondary,
   kernel_io11_primary,
   kernel_io11_secondary,
   kernel_io12_primary,
   kernel_io12_secondary,
   kernel_io13_primary,
   kernel_io_tms,
   kernel_io_tck,
   kernel_io_tdo,
   kernel_io_tdi,
   kernel_io_reset,
   kernel_io_rtck,
   kernel_io_external_total
};
#endif

typedef struct {
   sysfs_file_t i2c_file;
   pthread_mutex_t i2c_mutex;
} kernel_shared_t;

typedef struct {
   u8 peripheral_function;
   u8 io_flags; //input, output or unspecified
} kernel_shared_pin_state_t;

typedef struct {
   kernel_shared_pin_state_t pin_state[kernel_io_external_total];
   u8 pio_device_reference_count;
   u8 spi_device_reference_count;
   u8 uart_device_reference_count[5];
   u8 i2c_device_reference_count[3];
   u8 i2s_device_reference_count;
} kernel_shared_root_t;

int kernel_shared_init();

void kernel_shared_root_set_pin_state(
      u8 pin_number,
      u8 peripheral_function,
      u8 io_flags
      );

const kernel_shared_pin_state_t * kernel_shared_get_pin_state(
      u8 pin_number
      );

sysfs_file_t * kernel_shared_i2c_file();
pthread_mutex_t * kernel_shared_i2c_mutex();

#endif // KERNEL_SHARED_H
