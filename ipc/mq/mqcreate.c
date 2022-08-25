#include <pthread.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char **argv){
    if(argc!=2){
        fprintf(stderr, "usage: mqnotify <name>\n");
        _exit(-1);
    }

    struct mq_attr attr;
    attr.mq_maxmsg = 128;
    attr.mq_msgsize = 512;

    mqd_t mqd = mq_open(argv[1], 0x766, &attr);
    
    mq_close(mqd);

    _exit(0);
}
