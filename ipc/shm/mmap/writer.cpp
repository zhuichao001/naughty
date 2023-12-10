#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <memory.h>
#include <errno.h>
#include <unistd.h>

#define MMAP_DATA_SIZE 4096

int main(int argc,char* argv[]) {
    char* data;
    int fd = shm_open("shm2", O_CREAT|O_RDWR, 0777);
    int fd2 = shm_open("shm3", O_CREAT|O_RDWR, 0777);
    //int fd = open("/dev/shm/shm2", O_CREAT|O_RDWR, 0777);
    if (fd < 0) {
        printf("shm_open failed!\n");
        return -1;
    }

    ftruncate(fd, MMAP_DATA_SIZE);
    data = (char*)mmap(NULL, MMAP_DATA_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (!data) {
        printf("mmap failed\n");
        close(fd);
    }
    const char * msg = "This is a share memory! \n";
    memcpy(data, msg, strlen(msg));
    sleep(10);


    ftruncate(fd, MMAP_DATA_SIZE*2);
    data = (char*)mmap(NULL, MMAP_DATA_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, MMAP_DATA_SIZE);

    sprintf(data, "hhhhhhhhhhhhhh! %d\n", fd);

    sleep(10);
    munmap(data, MMAP_DATA_SIZE);

    close(fd); 
    return 0;
}
