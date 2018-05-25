#include "socket.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

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

int sock_connect(char* host, char* port, int socktype, int proto) {
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
        if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;
        close(sockfd);
    }

    freeaddrinfo(result);

    if (rp == NULL) {
        return -1;
    }
    return sockfd;
}
