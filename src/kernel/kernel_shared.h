#ifndef KERNEL_SHARED_H
#define KERNEL_SHARED_H

#include <sos/fs/sysfs.h>
#include <pthread.h>

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

typedef struct {
   sysfs_file_t i2c_file;
   pthread_mutex_t i2c_mutex;
} kernel_shared_t;

typedef struct {
   u8 core_peripheral[kernel_io_external_total];
} kernel_shared_root_t;



extern kernel_shared_t m_kernel_shared;
extern kernel_shared_root_t m_kernel_shared_root;

int kernel_shared_init();

#endif // KERNEL_SHARED_H
