/*POSIX record locks (fcntl).
**associated with an [i-node, pid] pair.
**synchronize processes, but not threads.
*/

#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "util.h"


int wlock(struct flock *lock, int fd, int start, int len){
    lock->l_type = F_WRLCK; //F_RDLCK
    lock->l_whence = SEEK_SET;
    lock->l_start = start;
    lock->l_len = len;
    lock->l_pid = 0; //getpid()
    if (fcntl(fd, F_SETLKW, lock) == -1) {
        return -1;
    }
    return 0;
}

int rlock(struct flock *lock, int fd, int start, int len){
    lock->l_type = F_WRLCK; //F_RDLCK
    lock->l_whence = SEEK_SET;
    lock->l_start = start;
    lock->l_len = len;
    lock->l_pid = 0; //getpid();
    if (fcntl(fd, F_SETLK, lock) == -1) {
        return -1;
    }
    return 0;
}

int unlock(struct flock *lock, int fd){
    lock->l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, lock) == -1) {
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
    disp_wait(10);
    unlock(&fl, fd);

    wlock(&fl, fd, 20,10);
    fprintf(stderr, "write lock ok. wait 10s:");
    disp_wait(10);
    unlock(&fl, fd);

    close(fd);
}
