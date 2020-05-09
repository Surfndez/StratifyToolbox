#ifndef DEVFS_I2C_DEVICE_H
#define DEVFS_I2C_DEVICE_H

#include <mcu/pio.h>

int i2c_device_open(const devfs_handle_t * handle);
int i2c_device_close(const devfs_handle_t * handle);
int i2c_device_ioctl(const devfs_handle_t * handle, int request, void * ctl);
int i2c_device_read(const devfs_handle_t * handle, devfs_async_t * async);
int i2c_device_write(const devfs_handle_t * handle, devfs_async_t * async);



#endif // DEVFS_I2C_DEVICE_H
