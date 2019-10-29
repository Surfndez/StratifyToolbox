
#include <jansson/jansson_api.h>
#include <mbedtls_api.h>
#include <sapi/sg.h>
#include <sos/crypt_api.h>

const void * kernel_request_api(u32 request){
	switch(request){
#if !_IS_BOOT
		case JANSSON_API_REQUEST:
			return &jansson_api;
		case SGFX_API_REQUEST:
			return &sg_api;
		//case MBEDTLS_API_REQUEST:
			//return &mbedtls_api;
#endif
		case CRYPT_SHA256_API_REQUEST:
			return &tinycrypt_sha256_hash_api;
	}
	return 0;
}
