#include <pthread.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

mqd_t mqd;
struct mq_attr attr;
struct sigevent sigev;
void *buff;

static void sigusr1(int signo){
    ssize_t n;
    mq_notify(mqd, &sigev);
    n = mq_receive(mqd, (char*)buff, attr.mq_msgsize, NULL);
    fprintf(stderr, "SIGUSR1 received, read %ld bytes\n", n);
    return;
}

int main(int argc, char **argv){
    if(argc!=2){
        fprintf(stderr, "usage: mqnotify <name>\n");
        _exit(-1);
    }

    mqd = mq_open(argv[1], O_RDONLY);

    mq_getattr(mqd, &attr);
    buff = malloc(attr.mq_msgsize);

    signal(SIGUSR1, sigusr1);
    sigev.sigev_notify = SIGEV_SIGNAL;
    sigev.sigev_signo = SIGUSR1;
    mq_notify(mqd, &sigev);

    for(;;){
        pause();
    }

    _exit(0);
}
