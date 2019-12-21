#ifndef DEVFS_SPI_DEVICE_H
#define DEVFS_SPI_DEVICE_H

#include <mcu/spi.h>

int spi_device_open(const devfs_handle_t * handle);
int spi_device_close(const devfs_handle_t * handle);
int spi_device_ioctl(const devfs_handle_t * handle, int request, void * ctl);
int spi_device_read(const devfs_handle_t * handle, devfs_async_t * async);
int spi_device_write(const devfs_handle_t * handle, devfs_async_t * async);



#endif // DEVFS_SPI_DEVICE_H
