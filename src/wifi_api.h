#ifndef WIFI_API_H
#define WIFI_API_H

#include <sys/socket.h>
#include <sos/fs/devfs.h>

#include "lwip/netdb.h"
#include "lwip/netif.h"
#include "lwip/sockets.h"

typedef struct {
	sysfs_shared_config_t device_config; //make this the first member
	u8 hw_addr[6];
	u16 mtu /*! Default value should be 1500 */;
	const char * host_name;
	struct netif * network_interface_list;
	u16 network_interface_count;
	u16 max_packet_size;
	u8 * packet_buffer;
} wifi_api_config_t;

typedef struct {
	err_t (*init_function)(struct netif *);
	err_t (*input_function)(struct pbuf *, struct netif *);
} wifi_api_netif_t;

typedef struct {
	sysfs_shared_state_t device_state;
	const wifi_api_config_t * config;
} wifi_api_state_t;

int wifi_api_startup(const void *);
int wifi_api_accept(int s, struct sockaddr *addr, socklen_t *addrlen);
int wifi_api_bind(int s, const struct sockaddr *name, socklen_t namelen);
int wifi_api_shutdown(int s, int how);
int wifi_api_getpeername(int s, struct sockaddr *name, socklen_t *namelen);
int wifi_api_getsockname(int s, struct sockaddr *name, socklen_t *namelen);
int wifi_api_getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen);
int wifi_api_setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen);
int wifi_api_close(int s);
int wifi_api_connect(int s, const struct sockaddr *name, socklen_t namelen);
int wifi_api_listen(int s, int backlog);
int wifi_api_recv(int s, void *mem, size_t len, int flags);
int wifi_api_read(int s, void *mem, size_t len);
int wifi_api_recvfrom(int s, void *mem, size_t len, int flags,
					 struct sockaddr *from, socklen_t *fromlen);
int wifi_api_send(int s, const void *dataptr, size_t size, int flags);
int wifi_api_sendmsg(int s, const struct msghdr *message, int flags);
int wifi_api_sendto(int s, const void *dataptr, size_t size, int flags,
				  const struct sockaddr *to, socklen_t tolen);
int wifi_api_socket(int domain, int type, int protocol);
int wifi_api_write(int s, const void *dataptr, size_t size);
int wifi_api_writev(int s, const struct iovec *iov, int iovcnt);
int wifi_api_select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
				  struct timeval *timeout);
int wifi_api_ioctl(int s, long cmd, void *argp);
int wifi_api_fcntl(int s, int cmd, int val);
int wifi_api_fsync(int s);
struct hostent * wifi_api_gethostbyname(const char *name);
int wifi_api_gethostbyname_r(const char *name, struct hostent *ret, char *buf, size_t buflen, struct hostent **result, int *h_errnop);
void wifi_api_freeaddrinfo(struct addrinfo *ai);
int wifi_api_getaddrinfo(const char *nodename, const char *servname, const struct addrinfo *hints, struct addrinfo **res);
in_addr_t wifi_api_inet_addr(const char * cp);
char * wifi_api_inet_ntoa(struct in_addr in);
const char * wifi_api_inet_ntop(int af, const void * src, char * dst, socklen_t size);
int wifi_api_inet_pton(int af, const char * src, void * dst);


#define WIFI_DECLARE_CONFIG_STATE(name_value, network_interface_count_value, device_filesystem_value, device_name_value, host_name_value, mtu_value, max_packet_size_value, hw_addr_0_value, hw_addr_1_value, hw_addr_2_value, hw_addr_3_value, hw_addr_4_value, hw_addr_5_value) \
	wifi_api_netif_state_t name_value##_state; \
	struct netif name_value##_network_interface_list[network_interface_count_value]; \
	u8 name_value##_packet_buffer[max_packet_size_value]; \
	wifi_api_config_t name_value##_config = { \
	.device_config.devfs = device_filesystem_value, .device_config.name = device_name_value, .device_config.state = name_value##_state, \
	.hw_addr[0] = hw_addr_0_value, .hw_addr[1] = hw_addr_1_value, .hw_addr[2] = hw_addr_2_value, .hw_addr[3] = hw_addr_3_value, .hw_addr[4] = hw_addr_4_value, .hw_addr[5] = hw_addr_5_value, \
	.mtu = mtu_value, \
	.max_packet_size = max_packet_size_value, \
	.netif_device_attr = netif_device_attr_value, \
	.host_name = host_name_value, \
	.network_interface_list = name_value##_network_interface_list, \
	.network_interface_count = network_interface_count_value }


#define WIFI_DECLARE_SOCKET_API(api_name, api_config, api_state) \
	const sos_socket_api_t api_name##_api = { \
	.startup = wifi_api_startup, \
	.accept = wifi_api_accept, \
	.bind = wifi_api_bind, \
	.shutdown = wifi_api_shutdown, \
	.getpeername = wifi_api_getpeername, \
	.getsockname = wifi_api_getsockname, \
	.getsockopt = wifi_api_getsockopt, \
	.setsockopt = wifi_api_setsockopt, \
	.close = wifi_api_close, \
	.connect = wifi_api_connect, \
	.read = wifi_api_read, \
	.listen = wifi_api_listen, \
	.recv = wifi_api_recv, \
	.recvfrom = wifi_api_recvfrom, \
	.send = wifi_api_send, \
	.sendmsg = wifi_api_sendmsg, \
	.sendto = wifi_api_sendto, \
	.socket = wifi_api_socket, \
	.write = wifi_api_write, \
	.writev = wifi_api_writev, \
	.select = wifi_api_select, \
	.ioctl = wifi_api_ioctl, \
	.fcntl = wifi_api_fcntl, \
	.fsync = 0, \
	.gethostbyname = wifi_api_gethostbyname, \
	.gethostbyname_r = wifi_api_gethostbyname_r, \
	.freeaddrinfo = wifi_api_freeaddrinfo, \
	.getaddrinfo = wifi_api_getaddrinfo, \
	.inet_addr = wifi_api_inet_addr, \
	.inet_ntoa = wifi_api_inet_ntoa, \
	.inet_ntop = wifi_api_inet_ntop, \
	.inet_pton = wifi_api_inet_pton, \
	.config = api_config, \
	.state = api_state }

#endif /* WIFI_API_H */
