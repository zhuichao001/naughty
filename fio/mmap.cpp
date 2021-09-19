#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include "hlc/watch.h"

const char *kFileName = "test.txt";

void init_file(){
    char c[21] = "This is text data. \n";
    FILE *fp = fopen(kFileName, "a+");
    for(int i=0; i<20000000; ++i){
        fwrite(c, sizeof(char), strlen(c), fp);
    }
    fclose(fp);
}

void release(int fd, char *mem, int size){
    if (fd > 0) {
        close(fd);
    }

    if (mem != (char *)-1) {
        munmap(mem, size);
    }
}

int test_mmap(){
    int fd = open(kFileName, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "open %s fail\n", kFileName);
        exit(-1);
    }

    struct stat sb;
    if (stat(kFileName, &sb) < 0) {
        fprintf(stderr, "stat %s fail\n", kFileName);
        close(fd);
        return -1;
    }

    char *mem = (char *)mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem == (char *)-1) {
        fprintf(stderr, "mmap fail\n");
        close(fd);
        return -1;
    }

    close(fd);

    char buf[1024];
    for(int i=0; i<sb.st_size-1024; i+=1024){
        memcpy(buf, mem+i, 1024);
    }

    munmap(mem, sb.st_size);
    return 0;
}

int test_read(){
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

    char buf[1024];
    for(int i=0; i<sb.st_size-1024; i+=1024){
        lseek(fd, i, SEEK_SET);
        if(read(fd, buf, 1024) == -1) { //atomicly lseek+rad
            perror("read failed");
            close(fd);
            return -1;
        }
    }
    close(fd);
    return 0;
}

int main(int argc, char *argv[]) {
    init_file();

    {
        watch_t t(Precision::MILISECOND);
        int64_t start = t.now();
        test_read();
        std::cout << "read cost:" << t.now()-start << "ms" << std::endl;
    }

    {
        watch_t t(Precision::MILISECOND);
        int64_t start = t.now();
        test_mmap();
        std::cout << "mmap read cost:" << t.now()-start << "ms" << std::endl;
    }

    return 0;
}
