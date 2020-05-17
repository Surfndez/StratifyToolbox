
#include <jansson/jansson_api.h>
#include <mbedtls_api.h>
#include <sapi/sg.h>
#include <sos/api/crypt_api.h>
#include <ToolboxAPI/toolbox_io.h>
#include <ToolboxAPI/toolbox_touch.h>
#include <ToolboxAPI/toolbox_app.h>

#include "kernel_io.h"
#include "kernel_touch.h"
#include "kernel_app.h"

int kernel_request(int request, void * args){
#if !_IS_BOOT
	if( request == TOOLBOX_IO_REQUEST ){
		return kernel_io_request(args);
	}

	if( request == TOOLBOX_TOUCH_REQUEST ){
		return kernel_touch_request(args);
	}

	if( request == TOOLBOX_APP_REQUEST ){
		return kernel_app_request(args);
	}
#endif
	return 0;
}

const void * kernel_request_api(u32 request){
	switch(request){
#if !_IS_BOOT
		case JANSSON_API_REQUEST:
			return &jansson_api;
		case SGFX_API_REQUEST:
			return &sg_api;
#if _IS_QSPI
		case MBEDTLS_API_REQUEST:
			return &mbedtls_api;
#endif
			//switch this overt to the hardware crypt unit
		case CRYPT_SHA256_API_REQUEST:
			return &tinycrypt_sha256_hash_api;
#endif
	}
	return 0;
}
