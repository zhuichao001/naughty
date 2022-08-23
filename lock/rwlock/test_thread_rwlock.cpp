#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "thread_rwlock.h"

int counter=0;
thread_rwlock_t rwlock;

const int N_READER=3, N_WRITER=5;
const int LOOP = 10000;

void *write_var(void *arg){
    for(int i=0; i<LOOP; ++i){
        thread_rwlock_wrlock(&rwlock);
        counter += 1;
        fprintf(stderr, "Write Thread(%x) counter=%d\n", (int)pthread_self(), counter);
        usleep(100);
        thread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    return NULL;
}


void *read_var(void *arg){
    while(1) {
        thread_rwlock_rdlock(&rwlock);
        fprintf(stderr, "Read Thread(%x) counter=%d\n", (int)pthread_self(), counter);
        usleep(100);

        if(counter==LOOP*N_WRITER){ //finished
            break;
        }
        thread_rwlock_unlock(&rwlock);
    }
    return NULL;
}


int main() {
    thread_rwlock_init(&rwlock, NULL);

    pthread_t tid[N_READER+N_WRITER];    
    for(int i=0; i<N_READER; i++){
        pthread_create(&tid[i], NULL, read_var, NULL); }

    for(int i=0; i<N_WRITER; i++){
        pthread_create(&tid[i+3], NULL, write_var, NULL);
    }

    for(int i=0; i<N_READER+N_WRITER; i++){
        pthread_join(tid[i], NULL);
    }

    thread_rwlock_destroy(&rwlock);    
    return 0;
}
