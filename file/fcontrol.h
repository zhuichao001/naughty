#include <fcntl.h>
#include <stdio.h>

int set_flag(int fd, int flags){
    int val;
    if((val=fcntl(fd, F_GETFL, 0)) < 0){
        fprintf(stderr, "fcntl F_GETFL error\n");
        return -1;
    }

    val |= flags;

    if(fcntl(fd, F_SETFL, val) < 0){
        fprintf(stderr, "fcntl F_SETFL error\n");
        return -1;
    }
    return 0;
}

int clear_flag(int fd, int flags){
    int val;
    if((val=fcntl(fd, F_GETFL, 0)) < 0){
        fprintf(stderr, "fcntl F_GETFL error\n");
        return -1;
    }

    val &= ~flags;

    if(fcntl(fd, F_SETFL, val) < 0){
        fprintf(stderr, "fcntl F_SETFL error\n");
        return -1;
    }
    return 0;
}
