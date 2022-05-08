#include <fcntl.h>
#include <stdio.h>
#include "fio.h"

int test_hole(){
    int fd = fio::fcreate("hole.txt");
    if(fd<0){
        return -1;
    }

    char buf[] = "abcdefghij";
    char buf2[] = "ABCDEFGHIJ";

    if(fio::fwrite(fd, buf, 10)!=10){
        return -1;
    }

    if(lseek(fd, 18384, SEEK_SET) == -1){
        return -1;
    }

    if(fio::fwrite(fd, buf2, 10) != 10){
        exit(-1);
    }

    fio::fclose(fd);
    return 0;
}

int test_nohole(){
    int fd = fio::fcreate("nohole.txt");
    if(fd<0){
        return -1;
    }

    char buf[18394];
    if(fio::fwrite(fd, buf, 18394)!=18394){
        return -1;
    }

    fio::fclose(fd);
    return 0;
}

/**
 * ls -ls hole.txt nohole.txt
 * od -c hole.txt
 * od -c nohole.txt
 */
int main(){
    if(test_hole()<0){
        fprintf(stderr, "failed test hole\n");
    }

    if(test_nohole()<0){
        fprintf(stderr, "failed test nohole\n");
    }
    return 0;
}
