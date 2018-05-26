#include "socket.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

struct timeval tv;

int cidr_to_in_addr(char* cidr, struct in_addr** all_ips, uint32_t* mask) {
    uint8_t first, second, third, fourth, range;
    uint32_t ip;
    struct in_addr ip_first, ip_last;
    int len, i;
    if (sscanf(cidr, "%hhu.%hhu.%hhu.%hhu/%hhu", &first, &second, &third, &fourth, &range) < 5) {
        return -1;
    }
    ip = (first << 24) | (second << 16UL) | (third << 8UL) | (fourth);
    *mask = (0xFFFFFFFFUL << (32 - range)) & 0xFFFFFFFFUL;

    ip_first.s_addr = ip & (*mask);
    ip_last.s_addr = ip_first.s_addr | ~(*mask);
    len = ip_last.s_addr - ip_first.s_addr + 1; 

    *all_ips = malloc((len) * sizeof(struct in_addr));
    for(i = 0; i < len; i++) {
        (*all_ips)[i].s_addr = htonl(ip_first.s_addr + i);
    }
    return len;
}

int fast_connect(int sfd, const struct sockaddr* addr, socklen_t addrlen) {
    fd_set fdset;

    setblocking(sfd, 0);
    connect(sfd, addr, addrlen);

    FD_ZERO(&fdset);
    FD_SET(sfd, &fdset);

    if (select(sfd + 1, NULL, &fdset, NULL, &tv) == 1) {
        int so_error;
        socklen_t len = sizeof(so_error);
        getsockopt(sfd, SOL_SOCKET, SO_ERROR, &so_error, &len);
        return so_error;
    }
    return -1;
}

int tcp_connect(char* host, char* port){
    return sock_create(host, port, SOCK_STREAM, IPPROTO_TCP, connect);
}

int tcp_connect_to(char* host, char* port, int sec, int usec){
    extern struct timeval tv;
    tv.tv_sec = sec;
    tv.tv_usec = usec;
    return sock_create(host, port, SOCK_STREAM, IPPROTO_TCP, fast_connect);
}

int udp_connect(char* host, char* port){
    return sock_create(host, port, SOCK_DGRAM, IPPROTO_UDP, connect);
}

int setblocking(int sock, int blocking) {
    int flags;
    if ((flags = fcntl(sock, F_GETFL, 0)) == -1) {
        return -1;
    }
    if (blocking)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;
    if (fcntl(sock, F_SETFL, flags) == -1) {
        return -1;
    }
    return 0;
}

int sock_create(
        char* host, 
        char* port, 
        int socktype, 
        int proto, 
        int (*handle_func)(int, const struct sockaddr*, socklen_t)) {

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int rint, sockfd;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = socktype;
    hints.ai_flags = 0;
    hints.ai_protocol = proto;
    
    rint = getaddrinfo(host, port, &hints, &result);
    if (rint != 0) {
        return -1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sockfd == -1)
            continue;

        if (handle_func == NULL || handle_func(sockfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;

        close(sockfd);
    }

    freeaddrinfo(result);

    if (rp == NULL) {
        return -1;
    }
    return sockfd;
}
