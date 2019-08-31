
#include <jansson/jansson_api.h>
#include <mbedtls_api.h>
#include <sapi/sg.h>

const void * kernel_request_api(u32 request){
#if !_IS_BOOT
	switch(request){
		case JANSSON_API_REQUEST:
			return &jansson_api;
		case SGFX_API_REQUEST:
			return &sg_api;
		//case MBEDTLS_API_REQUEST:
			//return &mbedtls_api;
	}
#endif
	return 0;
}
