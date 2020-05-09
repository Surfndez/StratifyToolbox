#ifndef SWD_DEVICE_H
#define SWD_DEVICE_H

#include <mcu/pio.h>
#include <mcu/tmr.h>

int swd_device_open(const devfs_handle_t * handle);
int swd_device_close(const devfs_handle_t * handle);
int swd_device_ioctl(const devfs_handle_t * handle, int request, void * ctl);
int swd_device_read(const devfs_handle_t * handle, devfs_async_t * async);
int swd_device_write(const devfs_handle_t * handle, devfs_async_t * async);


#endif // SWD_DEVICE_H
