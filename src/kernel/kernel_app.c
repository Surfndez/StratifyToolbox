#include <pthread.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sos/sos.h>
#include "kernel_shared.h"
#include "kernel_app.h"


static void clean_applications();
static int launch_application(const char * path, const char * args);
static void launch_home(const char * args);
static int update_progress(const void * context, int value, int total){

	return 0;
}


int kernel_app_init(){
	launch_home(NULL);
	return 0;
}

int kernel_app_request(void * args){
	toolbox_app_request_t * pargs = args;
	if( pargs->o_flags & TOOLBOX_APP_FLAG_LAUNCH ){
		const toolbox_app_request_t * app_request = kernel_shared_app_request();
		pargs->pid = app_request->pid;
		pargs->o_flags |= TOOLBOX_APP_FLAG_IS_PENDING;
		mcu_debug_log_info(
					MCU_DEBUG_USER0,
					"Received app request %s",
					pargs->path);
		kernel_shared_set_app_request(pargs);
	}
	return 0;
}

int kernel_app_update(){

	//check the status of the currently executing application
	const toolbox_app_request_t * app_request = kernel_shared_app_request();
	if( app_request->o_flags & TOOLBOX_APP_FLAG_IS_PENDING ){
		int current_pid = app_request->pid;
		toolbox_app_request_t update_app_request;

		if( current_pid > 0 && (kill(current_pid, 0) < 0) ){
			//current pid has stopped, it needs to be removed to make
			//space for the new application
			mcu_debug_log_info(MCU_DEBUG_USER0, "Cleaning applications");
			clean_applications();

			update_app_request.pid = -1;
			update_app_request.o_flags = 0;

			//launch a new application if it is available
			int next_pid = -1;
			mcu_debug_log_info(MCU_DEBUG_USER0, "Launching application %s", app_request->path);
			next_pid = launch_application(
						app_request->path,
						app_request->args
						);
			if( next_pid > 0 ){
				memcpy(&update_app_request,
							 app_request,
							 sizeof(update_app_request));

				update_app_request.pid = next_pid;
				update_app_request.o_flags = TOOLBOX_APP_FLAG_IS_RUNNING;
			}

			kernel_shared_set_app_request(&update_app_request);


		}
	}


	//launch /Home if no application is ready

	return 0;
}

void clean_applications(){
	DIR * dir;

	dir = opendir("/app/ram");
	if( dir == 0 ){
		//ASSERT a big problem
		return;
	}

	struct dirent entry;
	struct dirent * result;
	char path[PATH_MAX];

	do {
		readdir_r(dir, &entry, &result);
		if( result != NULL ){
			if( entry.d_name[0] != '.' ){
				/*! \TODO check for bg process -- don't delete those if they are running */
				strncpy(path, "/app/ram/", PATH_MAX-1);
				strncat(path, entry.d_name, PATH_MAX-1);
				mcu_debug_printf("Deleting %s\n", path);
				if( unlink(path) < 0 ){
					mcu_debug_printf("failed to delete (%d)\n", errno);
				}
			}
		}
	} while( result != NULL );
	closedir(dir);
}

void launch_home(const char * args){
	int pid = launch_application("/home/Home", NULL);
	if( pid > 0 ){
		toolbox_app_request_t update_app_request;
		strncpy(update_app_request.path, "/home/Home", TOOLBOX_APP_MAX_PATH_LENGTH);
		update_app_request.args[0] = 0;
		update_app_request.o_flags = TOOLBOX_APP_FLAG_IS_RUNNING;
		update_app_request.pid = pid;
		kernel_shared_set_app_request(&update_app_request);
	}
}

int launch_application(const char * path, const char * args){
	int pid;
	char exec_destination[PATH_MAX];
	pid = launch(
				path,
				exec_destination,
				args,
				0,
				0,
				update_progress,
				NULL,
				NULL
				);

	if( pid  < 0 ){
		mcu_debug_log_error(MCU_DEBUG_USER0, "Failed to launch %s\n", path);
	}
	return pid;
}
