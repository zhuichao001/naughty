
/* Locks created by flock() are associated with an open file table entry.
 *
 *
 */
#include <sys/file.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "util.h"


typedef int bool;

int wlock(const char *path, bool unblocking){
    int fd;
    if((fd = open(path, O_CREAT|O_RDWR, 0666)) < 0){
        return -1;
    }

    int block_flag = unblocking!=0?LOCK_NB:0;
    if(flock(fd, LOCK_EX|block_flag) < 0) { //Don't block if locked
        perror("flock LOCK_EX.\n");
        close (fd);
        return -1;
    }
    return fd;
}

int rlock(const char *path){
    int fd;
    if((fd = open(path, O_RDONLY, 0666)) < 0){
        return -1;
    }

    if(flock(fd, LOCK_SH|LOCK_NB) < 0) { 
        perror("flock LOCK_SH.\n");
        close (fd);
        return -1;
    }
    return fd;
}

void unlock(int fd) {
    flock (fd, LOCK_UN);
    close (fd);
}


int main(){
    int fd = wlock("ab.txt", 0);
    try_write(fd);
    wait_enter();
    unlock(fd);
    return 0;
}
