
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <stdbool.h>
#include <string.h>


void *align_malloc(size_t align, size_t size){
    unsigned char *mem;
    int ret = posix_memalign((void **)&mem, align, size);
    if (ret) {
        perror("posix_memalign failed");
        return NULL;
    }
    return mem;
}

int dio_write() {
    const int ALIGN_SIZE = 512;
    const int BUF_SIZE = 1000;
    unsigned char *buf = align_malloc(ALIGN_SIZE, BUF_SIZE);
    memset(buf, 'c', BUF_SIZE);

    int fd = open("./data.dio", O_WRONLY | O_DIRECT | O_CREAT, 0755);
    if (fd < 0) {
        perror("open() failed");
        return -1;
    }

    int ret = write(fd, buf, ALIGN_SIZE);
    if (ret < 0) {
        perror("write() failed");
        free(buf);
        close(fd);
        return -1;
    }

    free(buf);
    close(fd);

    return 0;
}

int main(int argc, char * argv[]) {
    dio_write();
}
