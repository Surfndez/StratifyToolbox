#ifndef WIFI_DEVICE_H
#define WIFI_DEVICE_H

#include <sos/fs/devfs.h>

int wifi_device_open(const devfs_handle_t * handle);
int wifi_device_close(const devfs_handle_t * handle);
int wifi_device_ioctl(const devfs_handle_t * handle, int request, void * ctl);
int wifi_device_read(const devfs_handle_t * handle, devfs_async_t * async);
int wifi_device_write(const devfs_handle_t * handle, devfs_async_t * async);



#endif /* WIFI_DEVICE_H */
