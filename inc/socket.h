#ifndef _SOCKET_H_
#define _SOCKET_H_
#include <netinet/in.h>
/*
 * all_ips must be freed if return int != -1
 */
int sock_connect(char* host, char* port, int socktype, int proto);
int cidr_to_in_addr(char* cidr, struct in_addr** all_ips, uint32_t* mask);
#endif
