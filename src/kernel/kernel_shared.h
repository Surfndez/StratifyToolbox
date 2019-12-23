#ifndef KERNEL_SHARED_H
#define KERNEL_SHARED_H

#include <sos/fs/sysfs.h>
#include <pthread.h>

enum kernel_shared_direction_channels {
   kernel_shared_direction_channel0,
   kernel_shared_direction_channel1,
   kernel_shared_direction_channel2,
   kernel_shared_direction_channel3,
   kernel_shared_direction_channel4,
   kernel_shared_direction_channel5,
   kernel_shared_direction_channel6,
   kernel_shared_direction_channel7,
   kernel_shared_direction_channel8,
   kernel_shared_direction_channel9,
   kernel_shared_direction_channel10,
   kernel_shared_direction_channel11,
   kernel_shared_direction_channel12,
   kernel_shared_direction_channel13,
   kernel_shared_direction_channel_tms,
   kernel_shared_direction_channel_tck,
   kernel_shared_direction_channel_tdo,
   kernel_shared_direction_channel_tdi,
   kernel_shared_direction_channel_reset,
   kernel_shared_direction_channel_rtck,
   kernel_shared_direction_channel_first = kernel_shared_direction_channel0,
   kernel_shared_direction_channel_last = kernel_shared_direction_channel_rtck
};

typedef struct {
   sysfs_file_t i2c_file;
   pthread_mutex_t i2c_mutex;
} kernel_shared_t;

typedef struct MCU_PACK {
   u8 peripheral_function;
   u8 peripheral_port;
   u8 io_flags; //input, output or unspecified
} kernel_shared_direction_state_t;

typedef struct {
   kernel_shared_direction_state_t direction_state[kernel_shared_direction_channel_last+1];
   u8 pio_device_reference_count;
   u8 spi_device_reference_count;
   u8 uart_device_reference_count[5];
   u8 i2c_device_reference_count[3];
   u8 i2s_device_reference_count;
} kernel_shared_root_t;

int kernel_shared_init();

void kernel_shared_root_reference_i2s();
void kernel_shared_root_dereference_i2s();
void kernel_shared_root_reference_spi();
void kernel_shared_root_dereference_spi();
void kernel_shared_root_reference_uart(u8 port);
void kernel_shared_root_dereference_uart(u8 port);
void kernel_shared_root_reference_i2c(u8 port);
void kernel_shared_root_dereference_i2c(u8 port);

void kernel_shared_root_set_direction_state(
      u8 pin_number,
      u8 peripheral_function,
      u8 peripheral_port,
      u8 io_flags
      );

const kernel_shared_direction_state_t * kernel_shared_get_direction_state(
      enum kernel_shared_direction_channels pin_number
      );

sysfs_file_t * kernel_shared_i2c_file();
pthread_mutex_t * kernel_shared_i2c_mutex();

#endif // KERNEL_SHARED_H
