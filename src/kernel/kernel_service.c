
#include <mcu/debug.h>
#include <pthread.h>
#include <unistd.h>

#include "kernel_service.h"
#include "kernel_io.h"
#include "kernel_app.h"
#include "kernel_shared.h"
#include "i2c_internal.h"

#define RUN_KERNEL_APP 0

static void * kernel_service_thread_function(void * args);

int kernel_service_init(){
	if( kernel_shared_init() < 0 ){
		mcu_debug_log_fatal(
					MCU_DEBUG_USER0,
					"failed to init kernel shared"
					);
		return -1;
	}

	if( i2c_internal_init() < 0 ){
		mcu_debug_log_fatal(
					MCU_DEBUG_USER0,
					"failed to init internal I2C"
					);
		return -1;
	}

	if( kernel_io_init() < 0 ){
		mcu_debug_log_fatal(
					MCU_DEBUG_USER0,
					"failed to init kernel IO"
					);
		return -1;
	}

	//start a new thread that manages the kernel services
	if( kernel_service_start_thread(
				kernel_service_thread_function,
				NULL,
				4096,
				SCHED_OTHER,
				0
				) < 0 ){

	}


	//start the WIFI
#if 0
	if( sos_board_config.socket_api ){
		sos_board_config.socket_api->startup(
					sos_board_config.socket_api->config
					);
	}
#endif

#if RUN_KERNEL_APP
	if( kernel_app_init() < 0 ){
		mcu_debug_log_fatal(
					MCU_DEBUG_USER0,
					"failed to init kernel app"
					);
		return -1;
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
#if RUN_KERNEL_APP
		kernel_app_update();
#endif

		usleep(50000UL);

	}

	return 0;
}

int kernel_service_start_thread(
		void * (*thread_function)(void*),
		void * thread_argument,
		u32 stack_size,
		int scheduler_policy,
		int scheduler_priority
		){
	int result;
	pthread_attr_t attr;
	pthread_t tmp;

	pthread_attr_init(&attr);

	if( pthread_attr_setstacksize(&attr, stack_size) < 0 ){
		mcu_debug_log_error(MCU_DEBUG_SOCKET, "Failed to set stack size");
	}

#if 0
	if( pthread_attr_setschedpolicy(&attr, scheduler_policy) < 0 ){
		mcu_debug_log_error(MCU_DEBUG_SOCKET, "Failed to set policy");
	}

	struct sched_param param;
	param.sched_priority = scheduler_priority;

	if( pthread_attr_setschedparam(&attr, &param) < 0 ){
		mcu_debug_log_error(MCU_DEBUG_SOCKET, "Failed to set priority");
	}
#endif

	result = pthread_create(
				&tmp,
				&attr,
				thread_function,
				thread_argument
				);

	if( result == 0 ){
		return tmp;
	}

	return result;
}
