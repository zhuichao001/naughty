#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILE_MODE 0X766
#define MAP_SIZE 65536*4096

int main(int argc, char **argv){
    int flags = O_RDWR | O_CREAT;
    const char *path = "tmp.shm_create";
    int fd = shm_open(path, flags, FILE_MODE); 
    ftruncate(fd, MAP_SIZE);

    char *ptr = (char*)mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    shm_unlink(path);
    _exit(0);
}
