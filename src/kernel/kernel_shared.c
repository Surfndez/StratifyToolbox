
#include <sos/dev/i2c.h>
#include <mcu/debug.h>
#include <pthread.h>
#include <fcntl.h>

#include "kernel_shared.h"

kernel_shared_t m_kernel_shared;
kernel_shared_root_t m_kernel_shared_root MCU_SYS_MEM;

static void svcall_init(void * args);

int kernel_shared_init(){

	memset(&m_kernel_shared, 0, sizeof(m_kernel_shared));
	cortexm_svcall(svcall_init, 0);

	return 0;
}

void svcall_init(void * args){
	CORTEXM_SVCALL_ENTER();
	MCU_UNUSED_ARGUMENT(args);
	for(enum kernel_shared_direction_channels channel
			= first_kernel_shared_direction_channel;
			channel < last_kernel_shared_direction_channel+1;
			channel++){
		kernel_shared_direction_state_t * direction_state =
				m_kernel_shared_root.direction_state + channel;

		if( (channel == kernel_shared_direction_channel0) ||
				(channel == kernel_shared_direction_channel7) ){
			direction_state->peripheral_function = CORE_PERIPH_RESERVED;
		} else {
			direction_state->peripheral_function = CORE_PERIPH_PIO;
		}
		direction_state->peripheral_port = 0;
		direction_state->io_flags = 0;
	}
}

void kernel_shared_root_set_direction_state(
		enum kernel_shared_direction_channels pin_number,
		u8 peripheral_function,
		u8 peripheral_port,
		u8 io_flags,
		const char * description
		){
	kernel_shared_direction_state_t * direction_state =
			m_kernel_shared_root.direction_state + pin_number;
	direction_state->peripheral_function = peripheral_function;
	direction_state->io_flags = io_flags;
	direction_state->peripheral_port = peripheral_port;
	direction_state->description = description;
}

const kernel_shared_direction_state_t * kernel_shared_get_direction_state(
		enum kernel_shared_direction_channels pin_number
		){
	return m_kernel_shared_root.direction_state + pin_number;
}

sysfs_file_t * kernel_shared_i2c_file(){
	return &m_kernel_shared.i2c_file;
}

toolbox_touch_packet_t * kernel_shared_last_touch_packet(){
	return &m_kernel_shared.last_touch_packet;
}

const toolbox_app_request_t * kernel_shared_app_request(){
	return &m_kernel_shared_root.app_request;
}

void svcall_shared_set_app(void * args){
	CORTEXM_SVCALL_ENTER();
	mcu_debug_printf("Copy memory %p %d\n", &m_kernel_shared_root.app_request, sizeof(toolbox_app_request_t));
	memcpy(
				&m_kernel_shared_root.app_request,
				args,
				sizeof(toolbox_app_request_t)
				);
}

void kernel_shared_set_app_request(const toolbox_app_request_t * request){
	cortexm_svcall(svcall_shared_set_app, (void*)request);
}

pthread_mutex_t * kernel_shared_i2c_mutex(){
	return &m_kernel_shared.i2c_mutex;
}

static void release_peripheral_lock(
		u8 peripheral_function,
		u8 peripheral_port){
	for(enum kernel_shared_direction_channels channel = first_kernel_shared_direction_channel;
			channel < first_kernel_shared_direction_channel+1;
			channel++){
		kernel_shared_direction_state_t * direction_state =
				m_kernel_shared_root.direction_state + channel;

		if( (direction_state->peripheral_function == peripheral_function) &&
				(direction_state->peripheral_port == peripheral_port) ){
			direction_state->peripheral_function = CORE_PERIPH_PIO;
			direction_state->peripheral_port = 0;
		}

	}
}

void kernel_shared_root_reference_i2s(){
	m_kernel_shared_root.i2s_device_reference_count++;
}

void kernel_shared_root_dereference_i2s(){
	if( m_kernel_shared_root.i2s_device_reference_count ){
		m_kernel_shared_root.i2s_device_reference_count--;
		if( m_kernel_shared_root.i2s_device_reference_count == 0 ){
			//release the lock
			release_peripheral_lock(CORE_PERIPH_I2S, 0);
		}
	}
}

void kernel_shared_root_reference_spi(){
	m_kernel_shared_root.spi_device_reference_count++;
}

void kernel_shared_root_dereference_spi(){
	if( m_kernel_shared_root.spi_device_reference_count ){
		m_kernel_shared_root.spi_device_reference_count--;
		if( m_kernel_shared_root.spi_device_reference_count == 0 ){
			release_peripheral_lock(CORE_PERIPH_SPI, 0);
		}
	}
}

void kernel_shared_root_reference_uart(u8 port){
	m_kernel_shared_root.uart_device_reference_count[port]++;
}

void kernel_shared_root_dereference_uart(u8 port){
	if( m_kernel_shared_root.uart_device_reference_count[port] ){
		m_kernel_shared_root.uart_device_reference_count[port]--;
		if( m_kernel_shared_root.uart_device_reference_count[port] == 0 ){
			release_peripheral_lock(CORE_PERIPH_UART, port);
		}
	}
}

void kernel_shared_root_reference_i2c(u8 port){
	m_kernel_shared_root.i2c_device_reference_count[port]++;
}

void kernel_shared_root_dereference_i2c(u8 port){
	if( m_kernel_shared_root.i2c_device_reference_count[port] ){
		m_kernel_shared_root.i2c_device_reference_count[port]--;
		if( m_kernel_shared_root.i2c_device_reference_count[port] == 0 ){
			release_peripheral_lock(CORE_PERIPH_I2C, port);
		}
	}
}

