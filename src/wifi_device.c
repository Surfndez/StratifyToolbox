
#include <sos/dev/netif.h>
#include <sos/fs/sysfs.h>
#include <mcu/debug.h>
#include <errno.h>

int wifi_device_open(
		const devfs_handle_t * handle
		){

}

int wifi_device_close(
		const devfs_handle_t * handle
		){

}

int wifi_device_ioctl(
		const devfs_handle_t * handle,
		int request,
		void * ctl
		){

	//implement the sos/dev/wifi.h ioctl calls

}

int wifi_device_read(
		const devfs_handle_t * handle,
		devfs_async_t * async
		){

	//use sockets to read
	return SYSFS_SET_RETURN(ENOTSUP);
}

int wifi_device_write(
		const devfs_handle_t * handle,
		devfs_async_t * async
		){

	//use sockets to write
	return SYSFS_SET_RETURN(ENOTSUP);
}


