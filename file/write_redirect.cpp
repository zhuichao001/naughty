#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* Test redirect stdout
 * */

int main() {
    close(1); // close stdout
    int fd = open("./test_write_buffer.dat", O_WRONLY | O_CREAT, 0644);
    if(fd < 0) {
        perror("open");
        return 1;
    }

    printf("fd: %d\n", fd);
    fprintf(stdout, "hello world\n");
    fprintf(stdout, "hello world\n");
    fprintf(stdout, "hello world\n");
    fprintf(stdout, "hello world\n");

    //If the FD is closed before exit, the kernel can't find fd to flush buffer 
    close(fd);
    return 0;
}
