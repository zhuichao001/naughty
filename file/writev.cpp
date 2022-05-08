#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>

#include "hlc/watch.h"

const char *kFileName = "test.txt";
const int BUFLEN = 40960;


int do_writev(){
    char buf1[BUFLEN];
    char buf2[BUFLEN];
    memset(buf1, '*', BUFLEN);
    memset(buf2, '.', BUFLEN);

    struct iovec iov[2];
    iov[0].iov_base = buf1;
    iov[0].iov_len = BUFLEN;
    iov[1].iov_base = buf2;
    iov[1].iov_len = BUFLEN;

    int fd = open(kFileName, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    if(fd < 0){
        perror("open");
        return -1;
    }

    int n = writev(fd,iov,2);
    close(fd);
    return 0;
}

int do_write(){
    char buf1[BUFLEN];
    char buf2[BUFLEN];
    memset(buf1, '*', BUFLEN);
    memset(buf2, '.', BUFLEN);

    char buf[BUFLEN+BUFLEN];
    memcpy(buf, buf1, BUFLEN);
    memcpy(buf+BUFLEN, buf2, BUFLEN);

    int fd = open(kFileName, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    if(fd < 0){
        perror("open");
        return -1;
    }

    int n = write(fd, buf, BUFLEN+BUFLEN);
    close(fd);
    return 0;
}

int main(void){
    {
        watch_t t(Precision::MILISECOND);
        int64_t start = t.now();
        for(int i=0; i<200000; ++i){
            do_write();
        }
        std::cout << "write cost:" << t.now()-start << "ms" << std::endl;
    }

    {
        watch_t t(Precision::MILISECOND);
        int64_t start = t.now();
        for(int i=0; i<200000; ++i){
            do_writev();
        }
        std::cout << "writev cost:" << t.now()-start << "ms" << std::endl;
    }
    return 0;
}

