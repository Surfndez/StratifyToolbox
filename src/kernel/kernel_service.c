
#include <mcu/debug.h>
#include <pthread.h>
#include <unistd.h>

#include "kernel_service.h"
#include "kernel_io.h"
#include "kernel_shared.h"


typedef struct {
	sysfs_file_t i2c_file;
} kernel_service_t;

kernel_service_t m_state;

static void * kernel_service_thread_function(void * args);
static int start_kernel_service_thread();

int kernel_service_init(){

	if( kernel_shared_init() < 0 ){
		MCU_DEBUG_LINE_TRACE();
		return -1;
	}

	if( kernel_io_init() < 0 ){
		mcu_debug_log_error(
					MCU_DEBUG_USER0,
					"failed to init kernel IO"
					);
		return -1;
	}

	//start a new thread that manages the board IO functions


	//start the WIFI
#if 0
	if( sos_board_config.socket_api ){
		sos_board_config.socket_api->startup(
					sos_board_config.socket_api->config
					);
	}
#endif

	return 0;
}


void * kernel_service_thread_function(void * args){

	while(1){

		//look at battery charging

		//look at battery voltage

		//check the power button

		//manage the currently running application -- relaunch home if it crashes

		sleep(1);

	}

	return 0;
}

int start_kernel_service_thread(){
	int result;
	pthread_attr_t attr;
	pthread_t tmp;

	pthread_attr_init(&attr);
	const u32 stack_size = 2048;
	if( pthread_attr_setstacksize(&attr, stack_size) < 0 ){
		mcu_debug_log_error(MCU_DEBUG_USER0, "Failed to set stack size");
	}

	result = pthread_create(
				&tmp,
				&attr,
				kernel_service_thread_function,
				0
				);

	//panic here if result < 0

	return result;
}
