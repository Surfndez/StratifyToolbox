#ifndef RAM_DRIVE_H
#define RAM_DRIVE_H

#include <sos/fs/types.h>

int ram_drive_open(const devfs_handle_t * handle);
int ram_drive_ioctl(const devfs_handle_t * handle, int request, void * ctl);
int ram_drive_read(const devfs_handle_t * handle, devfs_async_t * async);
int ram_drive_write(const devfs_handle_t * handle, devfs_async_t * async);
int ram_drive_close(const devfs_handle_t * handle);

#endif // RAM_DRIVE_H
