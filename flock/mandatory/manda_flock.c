#include <sys/fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int wlock(struct flock *fl, int fd){
    fl->l_type = F_WRLCK;
    fl->l_whence = SEEK_SET;
    fl->l_start = 0;
    fl->l_len = 0;

    //cmd: F_SETLK, FSETLKW, FGETLK
    if(fcntl(fd, F_SETLKW, fl) == -1){
        perror("fcntl F_SETLKW");
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
    if (argc != 2) {
        fprintf(stderr, "usage: %s file\n", argv[0]);
        exit(1);
    }

    const char *path = argv[1];
    int fd = open(path, O_RDWR);
    if(fd == -1){
        perror("open");
        exit(-1);
    }

    struct flock lock = {};
    if(wlock(&lock, fd)<0){
        perror("wlock");
        exit(-1);
    }

    pause();

    unlock(&lock, fd);
    exit(0);
}
