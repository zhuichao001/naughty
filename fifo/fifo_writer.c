#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


int make_fifo(const char* fifokey){
    if(mkfifo(fifokey, 0644) == -1){
        perror("mkfifo error");
        return -1;
    }
    return 0;
}


int copy_and_write(int srcfd, int destfd){
    const BUFF_SIZE = 16;
    char buf[BUFF_SIZE];
    int n = 0;
    while((n = read(srcfd, buf, BUFF_SIZE))){
        //atomic: write_size < PIPE_BUF
        write(destfd, buf, n);
    }
    return 0;
}


int copy_to_fifo(const char* path, const char* fifokey){
    int srcfd = open(path, O_RDONLY);
    if(srcfd == -1){
        perror("open src error");
        return -1;
    }

    int destfd = open(fifokey,O_WRONLY);
    if(destfd == -1){
        perror("open fifo error");
        return -1;
    }

    copy_and_write(srcfd, destfd);

    close(srcfd);
    close(destfd);
    printf("write file success\n");
}


int main(int argc, char **argv) {
    if(argc<2){
        fprintf(stderr,"usage:%s srcfile\n",argv[0]);
        exit(EXIT_FAILURE);
    }else if(argc<3){
        make_fifo("fifo.key");
    }

    const char* path = argv[1];
    const char* fifokey = (argc==3)?argv[2]:"fifo.key";
    copy_to_fifo(path, fifokey);
    return 0;
}
