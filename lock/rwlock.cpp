#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int counter=0;
pthread_rwlock_t rwlock;

const int N_READER=3, N_WRITER=5;
const int LOOP = 10000;

void *write_var(void *arg){
    for(int i=0; i<LOOP; ++i){
        pthread_rwlock_wrlock(&rwlock);
        counter += 1;
        fprintf(stderr, "Write Thread(%x) counter=%d\n", (int)pthread_self(), counter);
        usleep(100);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    return NULL;
}


void *read_var(void *arg){
    while(1) {
        pthread_rwlock_rdlock(&rwlock);
        fprintf(stderr, "Read Thread(%x) counter=%d\n", (int)pthread_self(), counter);
        usleep(100);

        if(counter==LOOP*N_WRITER){ //finished
            break;
        }
        pthread_rwlock_unlock(&rwlock);
    }
    return NULL;
}


int main() {
    pthread_rwlockattr_t attr;
    pthread_rwlockattr_init(&attr);
    pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP); //avoid writer hungry
    //pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_READER_NP); //avoid reader hungry
    pthread_rwlock_init(&rwlock, &attr);

    pthread_t tid[N_READER+N_WRITER];    
    for(int i=0; i<N_READER; i++){
        pthread_create(&tid[i], NULL, read_var, NULL); }

    for(int i=0; i<N_WRITER; i++){
        pthread_create(&tid[i+3], NULL, write_var, NULL);
    }

    for(int i=0; i<N_READER+N_WRITER; i++){
        pthread_join(tid[i], NULL);
    }

    pthread_rwlock_destroy(&rwlock);    
    return 0;
}
