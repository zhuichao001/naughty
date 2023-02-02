#include <aio.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

const int SIZE_TO_READ = 16;

int main() {
    //step 0: init
    int fd = open(".data.txt", O_RDONLY, 0);
    if (fd == -1) {
        printf("Unable to open fd!\n");
        exit(EXIT_FAILURE);
    }

    //step 1: start read
    char* buffer = (char *)malloc(SIZE_TO_READ);
    struct aiocb cb;
    memset(&cb, 0, sizeof(struct aiocb));
    cb.aio_nbytes = SIZE_TO_READ;
    cb.aio_fildes = fd;
    cb.aio_offset = 0;
    cb.aio_buf = buffer;
    if (aio_read(&cb) == -1) {
        close(fd);
        printf("Unable to create request!\n");
        exit(EXIT_FAILURE);
    }

    //step 2: wait for completing
    printf("Request enqueued!\n");

    while(aio_error(&cb) == EINPROGRESS) {
        printf("Working...\n");
    }
    int numBytes = aio_return(&cb);
    if (numBytes != -1) {
        printf("aio_return Success, buffer:%s\n", cb.aio_buf);
    } else {
        printf("aio_return Error!\n");
    }

    //step 3: clear
    free(buffer);
    close(fd);

    return 0;
}
