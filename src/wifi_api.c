
#include <sos/dev/netif.h>
#include <sos/fs/sysfs.h>
#include <mcu/debug.h>
#include <errno.h>

#include <cortexm/cortexm.h>
#include <cortexm/task.h>

#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>

#include "socket/include/m2m_socket.h"
#include "driver/include/m2m_wifi.h"

static void * wifi_api_thread_function(void * args);

int wifi_api_startup(const void * config){

	//start the wifi_api_thread_function() thread

	return 0;
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
	return 0;
}

int wifi_api_writev(int s, const struct iovec *iov, int iovcnt){
	return 0;
}

int wifi_api_select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
						  struct timeval *timeout){
	return 0;
}

int wifi_api_ioctl(int s, long cmd, void *argp){
	return 0;
}

int wifi_api_fcntl(int s, int cmd, int val){
	return 0;
}

int wifi_api_fsync(int s){
	return 0;
}

struct hostent * wifi_api_gethostbyname(const char *name){
	return 0;
}

int wifi_api_gethostbyname_r(const char *name, struct hostent *ret, char *buf, size_t buflen, struct hostent **result, int *h_errnop){
	return 0;
}

void wifi_api_freeaddrinfo(struct addrinfo *ai){

}

int wifi_api_getaddrinfo(const char *nodename, const char *servname, const struct addrinfo *hints, struct addrinfo **res){
	return 0;
}
in_addr_t wifi_api_inet_addr(const char * cp){
	return 0;
}
char * wifi_api_inet_ntoa(struct in_addr in){
	return 0;
}
const char * wifi_api_inet_ntop(int af, const void * src, char * dst, socklen_t size){
	return 0;
}
int wifi_api_inet_pton(int af, const char * src, void * dst){
	return 0;
}

void * wifi_api_thread_function(void * args){

	//initialize the Wifi

	while( 1 ){
		m2m_wifi_handle_events(0);
		sleep(1); // this will be interrupted when an event happens

	}


	return 0;
}

