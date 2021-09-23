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
    int fd;
    if( (fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){
        perror("socket() error");
        exit(1);
    }

    struct sockaddr * svraddr = create_ipv4_addr("0.0.0.0", UDPPORT);
    if(bind(fd, (struct sockaddr *)svraddr, sizeof(struct sockaddr_in)) < 0){
        perror("bind() error");
        exit(1);
    }

    struct sockaddr_in addr;
    {
        char recvline[MAXLINE];
        unsigned int addrlen = sizeof(sockaddr_in);
        int n = recvfrom(fd, recvline, sizeof(recvline), 0, (struct sockaddr *) &addr, &addrlen);
        fprintf(stderr, "RECV %d types:%s\n", n, recvline);
    }

    {
        char sendline[MAXLINE];
        sprintf(sendline, "response-client.");
        int n = sendto(fd, sendline, sizeof(sendline), 0, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
        if( n < 0){
            perror("sendto() error");
            exit(1);
        }
        fprintf(stderr, "SEND %d types:%s\n", n, sendline);
    }

    close(fd);

    return 0;
}
