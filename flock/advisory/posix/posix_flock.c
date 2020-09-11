/*POSIX record locks (fcntl).
**associated with an [i-node, pid] pair.
**synchronize processes, but not threads.
*/

#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "util.h"


int wlock(struct flock *fl, int fd, int start, int len){
    fl->l_type = F_WRLCK; //F_RDLCK
    fl->l_whence = SEEK_SET;
    fl->l_start = start;
    fl->l_len = len;
    fl->l_pid = 0; //getpid()
    if(fcntl(fd, F_SETLKW, fl) == -1){
        return -1;
    }
    return 0;
}

int rlock(struct flock *fl, int fd, int start, int len){
    fl->l_type = F_WRLCK; //F_RDLCK
    fl->l_whence = SEEK_SET;
    fl->l_start = start;
    fl->l_len = len;
    fl->l_pid = 0; //getpid();
    if(fcntl(fd, F_SETLK, fl) == -1){
        perror("fcntl F_SETLK");
        return -1;
    }
    return 0;
}

int unlock(struct flock *fl, int fd){
    fl->l_type = F_UNLCK;
    if(fcntl(fd, F_SETLK, fl) == -1){
        perror("fcntl F_SETLK");
        return -1;
    }
    return 0;
}

int main(int argc, char **argv){
    const char* path = argv[1];
    int fd = open(path, O_WRONLY|O_CREAT);
    if(fd == -1) {
        printf("Unable to open the file\n");
        exit(1);
    }

    struct flock fl;
    rlock(&fl, fd, 0, 10);
    //try_write(fd); //verify write access
    fprintf(stderr, "read lock ok. wait 10s:");
    wait_time(10);
    unlock(&fl, fd);

    wlock(&fl, fd, 20,10);
    fprintf(stderr, "write lock ok. wait 10s:");
    wait_time(10);
    unlock(&fl, fd);

    close(fd);
}
