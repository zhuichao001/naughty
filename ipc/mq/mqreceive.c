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


    mqd_t mqd = mq_open(argv[1], O_RDONLY /*| O_NONBLOCK*/);

    unsigned int prio;
    struct mq_attr attr;
    mq_getattr(mqd, &attr);
    void *buff = malloc(attr.mq_msgsize);
    int n = mq_receive(mqd, (char*)buff, attr.mq_msgsize, &prio);

    fprintf(stderr, "mq receive: %d bytes, priority:%d\n", n, prio);

    _exit(0);
}
