#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


int read_from_fifo(const char* fifokey){
    int fd = open(fifokey, O_RDONLY);
    if(fd == -1){
        perror("open error");
        return -1;
    }

    const int BUFF_SIZE = 16;
    char buf[BUFF_SIZE+1];
    int n = 0;
    while((n = read(fd, buf, BUFF_SIZE))){
        buf[n]='\0';
        printf(":::%s\n", buf);
    }

    close(fd);
    unlink(fifokey);
}


int main(int argc, char **argv){
    if(argc != 2){
        fprintf(stderr,"usage:%s fifo\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    read_from_fifo(argv[1]);

    printf("read success\n");
    return 0;
}
