#ifndef DEVFS_LOGIC_DEVICE_H
#define DEVFS_LOGIC_DEVICE_H

#include <sos/fs/devfs.h>

int logic_device_open(const devfs_handle_t * handle);
int logic_device_close(const devfs_handle_t * handle);
int logic_device_ioctl(const devfs_handle_t * handle, int request, void * ctl);
int logic_device_read(const devfs_handle_t * handle, devfs_async_t * async);
int logic_device_write(const devfs_handle_t * handle, devfs_async_t * async);



#endif // DEVFS_LOGIC_DEVICE_H
