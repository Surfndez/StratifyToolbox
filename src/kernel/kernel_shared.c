
#include <sos/dev/i2c.h>
#include <mcu/debug.h>
#include <pthread.h>
#include <fcntl.h>

#include "kernel_shared.h"

kernel_shared_t m_kernel_shared;
kernel_shared_root_t m_kernel_shared_root MCU_SYS_MEM;

static void svcall_init(void * args);

void kernel_shared_root_set_direction_state(
      enum kernel_shared_direction_channels pin_number,
      u8 peripheral_function,
      u8 peripheral_port,
      u8 io_flags
      ){
   kernel_shared_direction_state_t * direction_state =
         m_kernel_shared_root.direction_state + pin_number;
   direction_state->peripheral_function = peripheral_function;
   direction_state->io_flags = io_flags;
   direction_state->peripheral_port = peripheral_port;
}

const kernel_shared_direction_state_t * kernel_shared_get_direction_state(
      enum kernel_shared_direction_channels pin_number
      ){
   return m_kernel_shared_root.direction_state + pin_number;
}

sysfs_file_t * kernel_shared_i2c_file(){
   return &m_kernel_shared.i2c_file;
}

pthread_mutex_t * kernel_shared_i2c_mutex(){
   return &m_kernel_shared.i2c_mutex;
}

static void release_peripheral_lock(
      u8 peripheral_function,
      u8 peripheral_port){
   for(enum kernel_shared_direction_channels channel = kernel_shared_direction_channel_first;
       channel < kernel_shared_direction_channel_first+1;
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

void svcall_init(void * args){
   CORTEXM_SVCALL_ENTER();
   MCU_UNUSED_ARGUMENT(args);
   for(enum kernel_shared_direction_channels channel
       = kernel_shared_direction_channel_first;
       channel < kernel_shared_direction_channel_first+1;
       channel++){
      kernel_shared_direction_state_t * direction_state =
            m_kernel_shared_root.direction_state + channel;

      direction_state->peripheral_function = CORE_PERIPH_PIO;
      direction_state->peripheral_port = 0;
      direction_state->io_flags = 0;
   }
}

int kernel_shared_init(){
	int result;

	cortexm_svcall(svcall_init, 0);

	pthread_mutexattr_t mutex_attr;

	pthread_mutexattr_init(&mutex_attr);

	pthread_mutex_init(
				&m_kernel_shared.i2c_mutex,
				&mutex_attr
				);

	mcu_debug_log_info(MCU_DEBUG_USER0, "init kernel service");
	m_kernel_shared.i2c_file.fs = &(sysfs_list[1]);
	m_kernel_shared.i2c_file.flags = O_RDWR;
	m_kernel_shared.i2c_file.loc = 0;
	m_kernel_shared.i2c_file.handle = NULL;

	result = sysfs_file_open(
				&m_kernel_shared.i2c_file,
				"i2c_internal",
				O_RDWR
				);

	if( result < 0 ){
		mcu_debug_log_error(
					MCU_DEBUG_USER0,
					"failed to init: internal I2C (%d, %d)",
					result, errno
					);
		return -1;
	}

	mcu_debug_log_info(
				MCU_DEBUG_USER0,
				"set I2C attr"
				);
	result = sysfs_file_ioctl(&m_kernel_shared.i2c_file, I_I2C_SETATTR, 0);
	if( result < 0 ){
		mcu_debug_log_error(
					MCU_DEBUG_USER0,
					"failed to set attributes: internal I2C"
					);
		return -1;
	}

	return 0;
}
