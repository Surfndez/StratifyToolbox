
#include <sos/dev/netif.h>
#include <sos/fs/sysfs.h>
#include <mcu/debug.h>
#include <errno.h>

#include <cortexm/cortexm.h>
#include <cortexm/task.h>

#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>

#include "kernel_service.h"
#include "wifi_api.h"
#include "socket/include/m2m_socket.h"
#include "driver/include/m2m_wifi.h"

static void * wifi_api_thread_function(void * args);
static int start_wifi_thread(const void * config);

int wifi_api_startup(const void * config){

	//start the wifi_api_thread_function() thread
	mcu_debug_printf("------------start wifi thread---------------\n");

	return start_wifi_thread(config);
}

int wifi_api_accept(int s, struct sockaddr *addr, socklen_t * addrlen){
	return m2m_accept(s, addr, addrlen);
}

int wifi_api_bind(int s, const struct sockaddr *name, socklen_t namelen){
	return m2m_bind(s, name, namelen);
}

int wifi_api_shutdown(int s, int how){
	return -1;
}

int wifi_api_getpeername(int s, struct sockaddr *name, socklen_t *namelen){
	return 0;
}

int wifi_api_getsockname(int s, struct sockaddr *name, socklen_t *namelen){
	return -1;
}

int wifi_api_getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen){
	return m2m_getsockopt(s, level, optname, optval, optlen);
}

int wifi_api_setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen){
	return m2m_setsockopt(s, level, optname, optval, optlen);
}

int wifi_api_close(int s){
	return m2m_close(s);
}

int wifi_api_connect(int s, const struct sockaddr *name, socklen_t namelen){
	return m2m_connect(
				s,
				name,
				namelen
				);
}

int wifi_api_listen(int s, int backlog){
	return m2m_listen(s, backlog);
}

int wifi_api_recv(int s, void *mem, size_t len, int flags){
	u32 timeout = 0; //?
	return m2m_recv(s, mem, len, timeout);
}

int wifi_api_read(int s, void *mem, size_t len){
	return wifi_api_recv(s, mem, len, 0);
}

int wifi_api_recvfrom(
		int s,
		void *mem,
		size_t len,
		int flags,
		struct sockaddr *from,
		socklen_t *fromlen
		){
	u32 timeout = 0; //??
	return m2m_recvfrom(s, mem, len, timeout);
}

int wifi_api_send(int s, const void *dataptr, size_t size, int flags){
	return 0;
}

int wifi_api_sendmsg(int s, const struct msghdr *message, int flags){
	return 0;
}

int wifi_api_sendto(
		int s,
		const void *dataptr,
		size_t size,
		int flags,
		const struct sockaddr *to,
		socklen_t tolen
		){
	return m2m_sendto(
				s,
				(void*)dataptr,
				size,
				flags,
				to,
				tolen
				);
}

int wifi_api_socket(int domain, int type, int protocol){
	return m2m_socket(domain, type, protocol);
}

int wifi_api_write(int s, const void *dataptr, size_t size){
	return wifi_api_send(
				s,
				dataptr,
				size,
				0);
}

int wifi_api_writev(int s, const struct iovec *iov, int iovcnt){
	return 0;
}

int wifi_api_select(
		int maxfdp1,
		fd_set *readset,
		fd_set *writeset,
		fd_set *exceptset,
		struct timeval *timeout
		){
	return 0;
}

int wifi_api_ioctl(
		int s,
		long cmd,
		void *argp
		){

	return 0;
}

int wifi_api_fcntl(
		int s,
		int cmd,
		int val
		){

	return 0;
}

int wifi_api_fsync(int s){
	return 0;
}

struct hostent * wifi_api_gethostbyname(const char *name){
	return m2m_gethostbyname((u8*)name);
}

int wifi_api_gethostbyname_r(
		const char *name,
		struct hostent *ret,
		char *buf,
		size_t buflen,
		struct hostent **result,
		int *h_errnop
		){

	return 0;
}

void wifi_api_freeaddrinfo(struct addrinfo *ai){

}

int wifi_api_getaddrinfo(
		const char *nodename,
		const char *servname,
		const struct addrinfo *hints,
		struct addrinfo **res
		){

	return 0;
}

in_addr_t wifi_api_inet_addr(
		const char * cp
		){

	return 0;
}
char * wifi_api_inet_ntoa(
		struct in_addr in
		){

	return 0;
}
const char * wifi_api_inet_ntop(
		int af,
		const void * src,
		char * dst,
		socklen_t size
		){

	return 0;
}

int wifi_api_inet_pton(
		int af,
		const char * src,
		void * dst
		){

	return 0;
}

void wifi_api_AppWifiCb(uint8 u8MsgType, void * pvMsg){

}

void wifi_api_tpfAppMonCb(
		tstrM2MWifiRxPacketInfo *pstrWifiRxPacket,
		uint8 * pu8Payload,
		uint16 u16PayloadSize
		){

	//for monitoring raw packets

}

void * wifi_api_thread_function(void * args){

	const wifi_api_config_t * wifi_config = args;

	//initialize the Wifi
	mcu_debug_printf("entered wifi thread\n");


	tstrWifiInitParam wifi_init_param;

	wifi_init_param.pfAppWifiCb = wifi_api_AppWifiCb;
	wifi_init_param.pfAppMonCb = wifi_api_tpfAppMonCb;
	wifi_init_param.strEthInitParam.pfAppWifiCb = 0;
	wifi_init_param.strEthInitParam.pfAppEthCb = 0;
	wifi_init_param.strEthInitParam.au8ethRcvBuf = 0;
	wifi_init_param.strEthInitParam.u16ethRcvBufSize = 0;
	wifi_init_param.strEthInitParam.u8EthernetEnable = 0;
	wifi_init_param.strEthInitParam.__PAD8__ = 0;

	int result = m2m_wifi_init(&wifi_init_param);

	mcu_debug_printf("enter wifi loop with result %d\n", result);


	while( 1 ){
		m2m_wifi_handle_events(0);
		sleep(1); // this will be interrupted when an event happens
	}


	return 0;
}

int start_wifi_thread(const void * config){

	int result;
	const wifi_api_config_t * wifi_config = config;

	u32 stack_size = wifi_config->thread_stack_size;
	if( stack_size == 0 ){
		stack_size = 2048;
	}

	return kernel_service_start_thread(
				wifi_api_thread_function,
				(void*)config,
				stack_size,
				SCHED_OTHER,
				0
				);

#if 0
	pthread_attr_t attr;
	pthread_t tmp;

	pthread_attr_init(&attr);

	u32 stack_size = wifi_config->thread_stack_size;
	if( stack_size == 0 ){
		stack_size = 2048;
	}

	if( pthread_attr_setstacksize(&attr, stack_size) < 0 ){
		mcu_debug_log_error(MCU_DEBUG_SOCKET, "Failed to set stack size");
	}

#if 0
	if( pthread_attr_setschedpolicy(&attr, SCHED_RR) < 0 ){
		mcu_debug_log_error(MCU_DEBUG_SOCKET, "Failed to set policy");
	}

	struct sched_param param;
	param.sched_priority = prio;

	if( pthread_attr_setschedparam(&attr, &param) < 0 ){
		mcu_debug_log_error(MCU_DEBUG_SOCKET, "Failed to set priority");
	}
#endif

	result = pthread_create(&tmp,
									&attr,
									wifi_api_thread_function,
									(void*)config);

	return result;
#endif
}

