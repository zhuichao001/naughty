#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFSIZE 32
#define NBUFF 8

struct {
    struct {
        char data[BUFFSIZE];
    } buff[NBUFF];
    sem_t mutex, nempty, nstored;
} shared;


void *produce(void*){
    for(int i=0; ; ){
        sem_wait(&shared.nempty);

        sem_wait(&shared.mutex);
        /* critical region */
        sem_post(&shared.mutex);

        { // load data
            for(int k=0; k<BUFFSIZE-1; ++k){
                shared.buff[i].data[k] = 'a'+i%26;
            }
            shared.buff[i].data[BUFFSIZE-1] = '\0';
        }

        if(++i>=NBUFF){
            i = 0;
        }

        sem_post(&shared.nstored); // 1 more stored item
    }
}

void *consume(void*){
    for(int i=0; ; ){
        sem_wait(&shared.nstored);

        sem_wait(&shared.mutex);
        /* critical region */
        sem_post(&shared.mutex);

        fprintf(stderr, "consume buff:%s\n", shared.buff[i].data);
        if(++i>=NBUFF){
            i=0;
        }
        sem_post(&shared.nempty); // 1 more empty slot
    }
}

int main(int argc, char **argv){
    pthread_t tid_produce, tid_consume;

    sem_init(&shared.mutex, 0, 1);
    sem_init(&shared.nempty, 0, NBUFF);
    sem_init(&shared.nstored, 0, 0);

    pthread_create(&tid_produce, NULL, produce, NULL);
    pthread_create(&tid_consume, NULL, consume, NULL);

    pthread_join(tid_produce, NULL);
    pthread_join(tid_consume, NULL);

    sem_destroy(&shared.mutex);
    sem_destroy(&shared.nempty);
    sem_destroy(&shared.nstored);
    _exit(0);
}
