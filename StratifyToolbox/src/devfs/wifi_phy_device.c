

#include <mcu/pio.h>
#include <mcu/spi.h>
#include <mcu/debug.h>
#include <cortexm/cortexm.h>

#include "wifi_phy_device.h"

//DMA is currently working, SPI has a FIFO so the DMA isn't too critical for performance
//DMA is probably not working because of cache
#if USE_SPI_DMA
#define mcu_spi_ioctl mcu_spi_dma_ioctl
#define mcu_spi_swap mcu_spi_dma_swap
#define mcu_spi_open mcu_spi_dma_open
#define mcu_spi_close mcu_spi_dma_close
#define mcu_spi_read mcu_spi_dma_read
#define mcu_spi_write mcu_spi_dma_write
#define mcu_spi_setattr mcu_spi_dma_setattr
#endif

static int initialize_phy(const devfs_handle_t * handle);
static void init_io(const wifi_phy_config_t * config);
static void assert_reset(const wifi_phy_config_t * config);
static void deassert_reset(const wifi_phy_config_t * config);
static void assert_chip_select(const wifi_phy_config_t * config);
static void deassert_chip_select(const wifi_phy_config_t * config);
static void assert_chip_enable(const wifi_phy_config_t * config);
static void deassert_chip_enable(const wifi_phy_config_t * config);
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
	spi_attr_t spi_attributes;

	switch(request){
		case I_WIFIPHY_GETVERSION: return 0x0300;
		case I_WIFIPHY_ASSERT_RESET:
			assert_reset(config);
			return 0;
		case I_WIFIPHY_DEASSERT_RESET:
			deassert_reset(config);
			return 0;
		case I_WIFIPHY_SET_FULL_DUPLEX:
			spi_attributes.o_flags = SPI_FLAG_SET_FULL_DUPLEX;
			return mcu_spi_setattr(handle, &spi_attributes);
		case I_WIFIPHY_SET_HALF_DUPLEX:
			spi_attributes.o_flags = SPI_FLAG_SET_HALF_DUPLEX;
			return mcu_spi_setattr(handle, &spi_attributes);
	}

	return mcu_spi_ioctl(handle, request, ctl);
}

int wifi_phy_device_read(
		const devfs_handle_t * handle,
		devfs_async_t * async
		){

	const wifi_phy_config_t * config = handle->config;
	wifi_state_t * state = handle->state;

	//this is the real deal, read and write starts now
	assert_chip_select(config);
	cortexm_delay_us(1);
	state->transfer_event_handler = async->handler;

	async->handler.callback = handle_transfer_complete;
	async->handler.context = (void*)handle;

	//all operations are full duplex, read just sets up and waits for write
	return mcu_spi_read(handle, async);
}

int wifi_phy_device_write(
		const devfs_handle_t * handle,
		devfs_async_t * async
		){

	const wifi_phy_config_t * config = handle->config;
	wifi_state_t * state = handle->state;

	//if read is active, it will handle the CS
	if( state->transfer_event_handler.callback == 0 ){
		assert_chip_select(config);
		cortexm_delay_us(1);
		state->transfer_event_handler = async->handler;

		async->handler.callback = handle_transfer_complete;
		async->handler.context = (void*)handle;
	}

	return mcu_spi_write(handle, async);
}

int handle_transfer_complete(
		void * context,
		const mcu_event_t * event
		){
	const devfs_handle_t * handle = context;
	const wifi_phy_config_t * config = handle->config;
	wifi_state_t * state = handle->state;

	cortexm_delay_us(1);
	deassert_chip_select(config);

	int result = 0;
	if( state->transfer_event_handler.callback ){
		result = state->transfer_event_handler.callback(
					state->transfer_event_handler.context,
					event
					);
	}

	state->transfer_event_handler.callback = 0;

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
	mcu_spi_open(handle);

	int result = mcu_spi_setattr(
				handle,
				0
				);
	if( result < 0 ){
		mcu_spi_close(handle);
		return result;
	}

	state->is_initialized = 1;
	return 0;
}

void init_io(const wifi_phy_config_t * config){
	pio_attr_t pio_attributes;
	pio_attributes.o_flags = PIO_FLAG_SET_OUTPUT;
	pio_attributes.o_pinmask = (1<<config->reset_pin_number);

	mcu_pio_open(&config->reset_handle);
	mcu_pio_setattr(&config->reset_handle, &pio_attributes);

	pio_attributes.o_pinmask = (1<<config->chip_select_pin_number);
	mcu_pio_open(&config->chip_select_handle);
	mcu_pio_setattr(&config->chip_select_handle, &pio_attributes);

	pio_attributes.o_pinmask = (1<<config->chip_enable_pin_number);
	mcu_pio_open(&config->chip_enable_handle);
	mcu_pio_setattr(&config->chip_enable_handle, &pio_attributes);

	deassert_chip_select(config);
	deassert_chip_enable(config);
	assert_reset(config);
	cortexm_delay_ms(10);

	assert_chip_enable(config);
	cortexm_delay_ms(10);

	deassert_reset(config);
	return;
}

void assert_reset(const wifi_phy_config_t * config){
	mcu_pio_clrmask(
				&config->reset_handle,
				(void*)(1<<config->reset_pin_number)
				);
}

void deassert_reset(const wifi_phy_config_t * config){
	mcu_pio_setmask(
				&config->reset_handle,
				(void*)(1<<config->reset_pin_number)
				);
}

void assert_chip_select(const wifi_phy_config_t * config){
	mcu_pio_clrmask(
				&config->chip_select_handle,
				(void*)(1<<config->chip_select_pin_number)
				);
}

void deassert_chip_select(const wifi_phy_config_t * config){
	mcu_pio_setmask(
				&config->chip_select_handle,
				(void*)(1<<config->chip_select_pin_number)
				);
}

void assert_chip_enable(const wifi_phy_config_t * config){
	mcu_pio_setmask(
				&config->chip_enable_handle,
				(void*)(1<<config->chip_enable_pin_number)
				);
}

void deassert_chip_enable(const wifi_phy_config_t * config){
	mcu_pio_clrmask(
				&config->chip_enable_handle,
				(void*)(1<<config->chip_enable_pin_number)
				);
}
