#ifndef _SOCKET_H_
#define _SOCKET_H_
#include <netinet/in.h>
/*
 * all_ips must be freed if return int != -1
 */
int tcp_connect(char* host, char* port);
int tcp_connect_to(char* host, char* port, int sec, int usec);
int udp_connect(char* host, char* port);
int sock_create(char* host,
                char* port,
                int socktype,
                int proto,
                int (*handle_func)(int, const struct sockaddr*, socklen_t));
int cidr_to_in_addr(char* cidr, struct in_addr** all_ips, uint32_t* mask);
unsigned short port_range(char* portrange, unsigned short** ports);
int setblocking(int sock, int blocking);


#endif
