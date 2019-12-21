#ifndef DEVFS_UART_DEVICE_H
#define DEVFS_UART_DEVICE_H

#include <mcu/pio.h>

int uart_device_open(const devfs_handle_t * handle);
int uart_device_close(const devfs_handle_t * handle);
int uart_device_ioctl(const devfs_handle_t * handle, int request, void * ctl);
int uart_device_read(const devfs_handle_t * handle, devfs_async_t * async);
int uart_device_write(const devfs_handle_t * handle, devfs_async_t * async);


#endif // DEVFS_UART_DEVICE_H
