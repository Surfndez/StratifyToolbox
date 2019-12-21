#ifndef PIO_EXT_DEVICE_H
#define PIO_EXT_DEVICE_H

#include <mcu/pio.h>

int pio_device_open(const devfs_handle_t * handle);
int pio_device_close(const devfs_handle_t * handle);
int pio_device_ioctl(const devfs_handle_t * handle, int request, void * ctl);
int pio_device_read(const devfs_handle_t * handle, devfs_async_t * async);
int pio_device_write(const devfs_handle_t * handle, devfs_async_t * async);



#endif // PIO_EXT_DEVICE_H
