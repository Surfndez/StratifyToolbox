

#include <mcu/pio.h>
#include <mcu/spi.h>

#include "wifi_phy_device.h"


static int initialize_phy(const devfs_handle_t * handle);
static void init_io(const wifi_phy_config_t * config);
static void assert_reset(const wifi_phy_config_t * config);
static void deassert_reset(const wifi_phy_config_t * config);
static void assert_cs(const wifi_phy_config_t * config);
static void deassert_cs(const wifi_phy_config_t * config);
static int handle_transfer_complete(void * context, const mcu_event_t * event);

int wifi_phy_device_open(
		const devfs_handle_t * handle
		){
	return initialize_phy(handle);
}

int wifi_phy_device_close(
		const devfs_handle_t * handle
		){

	//todo -- shut down the wifi device
	return 0;
}

int wifi_phy_device_ioctl(
		const devfs_handle_t * handle,
		int request,
		void * ctl
		){

	const wifi_phy_config_t * config = handle->config;

	switch(request){
		case I_WIFIPHY_GETVERSION: return 0x0300;
		case I_WIFIPHY_ASSERT_RESET:
			assert_reset(config);
			return 0;
		case I_WIFIPHY_DEASSERT_RESET:
			deassert_reset(config);
			return 0;
	}

	return mcu_spi_dma_ioctl(handle, request, ctl);
}

int wifi_phy_device_read(
		const devfs_handle_t * handle,
		devfs_async_t * async
		){

	const wifi_phy_config_t * config = handle->config;
	//all operations are full duplex, read just sets up and waits for write
	return mcu_spi_dma_read(handle, async);
}

int wifi_phy_device_write(
		const devfs_handle_t * handle,
		devfs_async_t * async
		){

	const wifi_phy_config_t * config = handle->config;
	wifi_state_t * state = handle->state;

	//this is the real deal, read and write starts now
	assert_cs(config);
	state->transfer_event_handler = async->handler;

	async->handler.callback = handle_transfer_complete;
	async->handler.context = (void*)handle;

	return mcu_spi_dma_write(handle, async);
}

int handle_transfer_complete(
		void * context,
		const mcu_event_t * event
		){
	const devfs_handle_t * handle = context;
	const wifi_phy_config_t * config = handle->config;
	wifi_state_t * state = handle->state;

	deassert_cs(config);

	int result = 0;
	if( state->transfer_event_handler.callback ){
		result = state->transfer_event_handler.callback(
					state->transfer_event_handler.context,
					&event
					);
	}
	return result;
}

int initialize_phy(
		const devfs_handle_t * handle
		){
	const wifi_phy_config_t * config = handle->config;
	wifi_state_t * state = handle->state;

	if( state->is_initialized ){
		return 0;
	}

	init_io(config);
	mcu_spi_dma_open(handle);

	int result = mcu_spi_dma_setattr(
				handle,
				0
				);
	if( result < 0 ){
		mcu_spi_dma_close(handle);
		return result;
	}

	state->is_initialized = 1;
	return 0;
}

void init_io(const wifi_phy_config_t * config){
	pio_attr_t pio_attributes;
	pio_attributes.o_flags = PIO_FLAG_SET_OUTPUT;
	pio_attributes.o_pinmask = (1<<config->reset.pin);

	mcu_pio_open(&config->reset_handle);
	mcu_pio_setattr(&config->reset_handle, &pio_attributes);

	pio_attributes.o_pinmask = (1<<config->cs.pin);
	mcu_pio_open(&config->cs_handle);
	mcu_pio_setattr(&config->cs_handle, &pio_attributes);

	deassert_reset(config);
	deassert_cs(config);
	return;
}

void assert_reset(const wifi_phy_config_t * config){
	mcu_pio_clrmask(
				&config->reset_handle,
				(void*)(1<<config->reset.pin)
				);
}

void deassert_reset(const wifi_phy_config_t * config){
	mcu_pio_setmask(
				&config->reset_handle,
				(void*)(1<<config->reset.pin)
				);
}

void assert_cs(const wifi_phy_config_t * config){
	mcu_pio_clrmask(
				&config->cs_handle,
				(void*)(1<<config->cs.pin)
				);
}

void deassert_cs(const wifi_phy_config_t * config){
	mcu_pio_setmask(
				&config->cs_handle,
				(void*)(1<<config->cs.pin)
				);
}
