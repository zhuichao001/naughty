#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "common/address.h"

#define MAXLINE 4096
#define UDPPORT 8001

int main(){
    int fd = 0;
    if( (fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){
        perror("socket() error");
        exit(1);
    }

    struct sockaddr * svraddr = create_ipv4_addr("127.0.0.1", UDPPORT);

    {
        char sendline[MAXLINE];
        sprintf(sendline,"request-server.");
        int n = sendto(fd, sendline, sizeof(sendline), 0, (struct sockaddr *)svraddr, sizeof(struct sockaddr_in));
        if(n < 0 ){
            perror("sendto() error");
            exit(1);
        }
        fprintf(stderr, "SEND %d types:%s\n", n, sendline);
    }

    {
        char recvline[MAXLINE];
        unsigned int addrlen = sizeof(sockaddr_in);
        int n = recvfrom(fd, recvline, sizeof(recvline), 0, (struct sockaddr *)svraddr, &addrlen);
        fprintf(stderr, "RECV %d types:%s\n", n, recvline);
    }

    close(fd);

    return 0;
}
