#ifndef KERNEL_SHARED_H
#define KERNEL_SHARED_H

#include <sos/fs/sysfs.h>
#include <pthread.h>

typedef struct {
   sysfs_file_t i2c_file;
   pthread_mutex_t i2c_mutex;
} kernel_shared_t;

extern kernel_shared_t m_kernel_shared;

int kernel_shared_init();

#endif // KERNEL_SHARED_H
