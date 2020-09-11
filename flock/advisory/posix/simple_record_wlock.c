/*lockf:simplified POSIX record locks.
**supports only exclusive locks
*/

#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>


int wlock(int fd, int start, int len){
    if (lseek(fd, start, SEEK_SET) == -1) {
        perror("lseek");
        return -1;
    }
    if (lockf(fd, F_LOCK, len) == -1) {
        perror("lockf");
        return -1;
    }
    return 0;
}

int unlock(int fd, int len){
    if (lockf(fd, F_ULOCK, len) == -1) {
        perror("lockf");
        return -1;
    }
    return 0;
}

void disp_wait(int seconds){
    for(int i=0; i<seconds; ++i){
        sleep(1);
        fprintf(stderr, ".");
    }
    fprintf(stderr, "\n");
}

void try_write(int fd){
    lseek(fd, 0, SEEK_SET);
    int err = write(fd, "RRR", 3);
    if(err<0){
        fprintf(stderr, "share mode write errno:%d\n", err);
        exit(1);
    }
}

int main(int argc, char **argv){
    const char* path = argv[1];
    int fd = open(path, O_WRONLY|O_CREAT);
    if(fd == -1) {
        printf("Unable to open the file\n");
        exit(1);
    }

    wlock(fd, 0, 10);
    try_write(fd);
    fprintf(stderr, "write lock ok. wait 10s:");
    disp_wait(10);
    unlock(fd, 10);

    close(fd);
}
