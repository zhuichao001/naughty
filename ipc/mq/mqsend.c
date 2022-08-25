#include <pthread.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


int main(int argc, char **argv){
    if(argc!=4){
        fprintf(stderr, "usage: mqsend <name> <#bytes> <priority>\n");
        _exit(-1);
    }

    size_t len = atoi(argv[2]);
    int prio = atoi(argv[3]);

    mqd_t mqd = mq_open(argv[1], O_WRONLY);
    void *ptr = calloc(len, sizeof(char));
    mq_send(mqd, (char*)ptr, len, prio);

    _exit(0);
}
