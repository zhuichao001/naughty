#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <linux/input.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>      
#include <fcntl.h> 

#define  MMAP_DATA_SIZE 4096
int main(int argc, char* argv[])
{
    char *data;
    int fd = shm_open("shm2", O_CREAT|O_RDWR, 0777);
    //int fd = shm_open("shm2", O_RDWR, 0777);
    if(fd < 0){
        perror("shm01 open error");
    }

    ftruncate(fd, MMAP_DATA_SIZE);
    data = (char*)mmap(NULL, MMAP_DATA_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(data == MAP_FAILED){
        perror("data error");
    }
    sleep(10);
    printf("data:%s\n", data);

    sleep(10);
    ftruncate(fd, MMAP_DATA_SIZE*2);
    data = (char*)mmap(NULL, MMAP_DATA_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, MMAP_DATA_SIZE);
    printf("open read 2nd.\n");
    printf("data:%s\n", data);
    munmap(data, MMAP_DATA_SIZE);
    close(fd);
    return 0;
}
