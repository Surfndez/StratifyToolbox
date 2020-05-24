#ifndef KERNEL_SERVICE_H
#define KERNEL_SERVICE_H

#include <pthread.h>
#include "config.h"

int kernel_service_init();
int kernel_service_start_thread(
		void * (*thread_function)(void*),
		void * thread_argument,
		u32 stack_size,
		int scheduler_policy,
		int scheduler_priority
		);

#endif // KERNEL_SERVICE_H
