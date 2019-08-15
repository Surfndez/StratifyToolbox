#include <sos/fs/sysfs.h>
#include <string.h>
#include "ram_drive.h"

#define RAM_START (void*)0x24000000
#define RAM_SIZE (512*1024)

int ram_drive_open(const devfs_handle_t * handle){
	MCU_UNUSED_ARGUMENT(handle);
	return 0;
}

int ram_drive_ioctl(const devfs_handle_t * handle, int request, void * ctl){
	MCU_UNUSED_ARGUMENT(handle);
	MCU_UNUSED_ARGUMENT(request);
	MCU_UNUSED_ARGUMENT(ctl);
	return SYSFS_SET_RETURN(ENOTSUP);
}

int ram_drive_read(const devfs_handle_t * handle, devfs_async_t * async){
	MCU_UNUSED_ARGUMENT(handle);
	if( async->loc + async->nbyte > RAM_SIZE ){
		async->nbyte = RAM_SIZE - async->loc;
	}

	memcpy(async->buf, RAM_START + async->loc, async->nbyte);
	return async->nbyte;
}

int ram_drive_write(const devfs_handle_t * handle, devfs_async_t * async){
	MCU_UNUSED_ARGUMENT(handle);
	if( async->loc + async->nbyte > RAM_SIZE ){
		async->nbyte = RAM_SIZE - async->loc;
	}

	memcpy(RAM_START + async->loc, async->buf, async->nbyte);
	return async->nbyte;
}

int ram_drive_close(const devfs_handle_t * handle){
	MCU_UNUSED_ARGUMENT(handle);
	return 0;
}
