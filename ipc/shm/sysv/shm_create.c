#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILE_MODE 0X766

int main(int argc, char **argv){
    int flags = O_RDWR | O_CREAT;
    int length = 1024;
    const char *path = "tmp.shm_create";
    int fd = shm_open(path, flags, FILE_MODE); 
    ftruncate(length, length);

    char *ptr = (char*)mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    shm_unlink(path);
    _exit(0);
}
