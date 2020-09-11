#include <stdio.h>

void disp_wait(int seconds){
    for(int i=0; i<seconds; ++i){
        sleep(1);
        fprintf(stderr, ".");
    }
    fprintf(stderr, "\n");
}

void enter_wait(){
    fprintf(stderr, "Press Enter:"); 
    char c;
    scanf("%c",&c);
}

void try_write(int fd){
    lseek(fd, 0, SEEK_SET);
    int err = write(fd, "RRR", 3);
    if(err<0){
        fprintf(stderr, "share mode write errno:%d\n", err);
        exit(1);
    }
}

