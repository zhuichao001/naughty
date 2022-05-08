#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int open_temporary(){

    char path[] = "template-XXXXXX";
    int fd = mkstemp(path);

    if(unlink(path) < 0){//remove() is ok
        fprintf(stderr, "unlink error\n");
        return -1;
    }

    fprintf(stderr, "file unlinked\n");
    return fd;
}

int main(){
    int fd = open_temporary();
    write(fd, "hello", 5);
    fsync(fd);

    sleep(30);
    printf("done");
    close(fd);
    return 0;
}
