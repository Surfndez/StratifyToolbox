
#include "swd_device.h"

int swd_device_open(const devfs_handle_t * handle){
   return SYSFS_SET_RETURN(ENOTSUP);
}

int swd_device_close(const devfs_handle_t * handle){
   return SYSFS_SET_RETURN(ENOTSUP);
}

int swd_device_ioctl(const devfs_handle_t * handle, int request, void * ctl){
   return SYSFS_SET_RETURN(ENOTSUP);
}

int swd_device_read(const devfs_handle_t * handle, devfs_async_t * async){
   return SYSFS_SET_RETURN(ENOTSUP);
}

int swd_device_write(const devfs_handle_t * handle, devfs_async_t * async){
   return SYSFS_SET_RETURN(ENOTSUP);
}

