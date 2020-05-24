
#include <mcu/debug.h>
#include <pthread.h>
#include <unistd.h>

#include "kernel_service.h"
#include "kernel_io.h"
#include "kernel_app.h"
#include "kernel_dac.h"
#include "kernel_shared.h"
#include "i2c_internal.h"

#define RUN_APP_SERVICE 0
#define RUN_DAC_SERVICE 0
#define RUN_WIFI_SERVICE 0
#define RUN_KERNEL_SERVICE 0
#define RUN_INTERNAL_IO 0

static void * kernel_service_thread_function(void * args);

int kernel_service_init(){
	if( kernel_shared_init() < 0 ){
		mcu_debug_log_fatal(
					MCU_DEBUG_USER0,
					"failed to init kernel shared"
					);
		return -1;
	}

#if RUN_INTERNAL_IO
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
#endif

#if ___debug
	//makes the UART TX available to the outside pin
	kernel_io_enable_uart_debug_tx();
	mcu_debug_log_info(MCU_DEBUG_USER0, "enabled UART out");
#endif

#if RUN_KERNEL_SERVICE
	//start a new thread that manages the kernel services
	if( kernel_service_start_thread(
				kernel_service_thread_function,
				NULL,
				4096,
				SCHED_OTHER,
				0
				) < 0 ){

	}
#endif


	//start the WIFI
#if RUN_WIFI_SERVICE
	if( sos_board_config.socket_api ){
		if( sos_board_config.socket_api->startup(
					sos_board_config.socket_api->config
					) < 0 ){
			mcu_debug_log_fatal(
						MCU_DEBUG_USER0,
						"failed to init dac service"
						);
		}
	}
#endif

#if RUN_DAC_SERVICE
	if( kernel_dac_init() < 0 ){
		mcu_debug_log_fatal(
					MCU_DEBUG_USER0,
					"failed to init dac service"
					);
		return -1;
	}
#endif

#if RUN_APP_SERVICE
	if( kernel_app_init() < 0 ){
		mcu_debug_log_fatal(
					MCU_DEBUG_USER0,
					"failed to init app service"
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

		mcu_debug_printf("looping\n");
		usleep(500000UL);

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
