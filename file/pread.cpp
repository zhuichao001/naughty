#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "hlc/watch.h"

/**
 * 
 * #include <unistd.h>
 * ssize_t pread(int fd, void *buf, size_t count, off_t offset);
 * ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset);
 * */

const char *kFileName = "test.txt";
int fileSize = 0;

void init_file(){
    char c[21] = "This is text data. \n";
    FILE *fp = fopen(kFileName, "a+");
    for(int i=0; i<1000; ++i){
        size_t n = fwrite(c, sizeof(char), strlen(c), fp);
        if(n>0){
            fileSize += n;
        }
    }
    fclose(fp);
}

int do_pread(int fd, int filesize){
    char buf[1024] = {0};
    for(int i=0; i<200000; ++i){
        int count = 1000;
        int offset = random()%(filesize-1000);
        if(pread(fd, buf, count, offset) == -1) { //atomicly lseek+rad
            perror("pread failed");
            return -1;
        }
    }
    return 0;
}

int do_read(int fd, int filesize){
    char buf[1024] = {0};
    for(int i=0; i<200000; ++i){
        int count = 1000;
        int offset = random()%(filesize-1000);
        lseek(fd, offset, SEEK_SET);
        if(read(fd, buf, count) == -1) {
            perror("read failed");
            return -1;
        }
    }
    return 0;
}

int main() {
    init_file();

    int fd = open(kFileName, O_RDONLY);
    if(fd < 0) {
        perror("open failed");
        return -1;
    }

    struct stat sb;
    if (stat(kFileName, &sb) < 0) {
        fprintf(stderr, "stat %s fail\n", kFileName);
        close(fd);
        return -1;
    }

    {
        watch_t t(Precision::MILISECOND);
        int64_t start = t.now();
        do_read(fd, sb.st_size);
        std::cout << "lseek+read cost:" << t.now()-start << "ms" << std::endl;
    }

    {
        watch_t t(Precision::MILISECOND);
        int64_t start = t.now();
        do_pread(fd, sb.st_size);
        std::cout << "pread cost:" << t.now()-start << "ms" << std::endl;
    }

    close(fd);
    return 0;
}
