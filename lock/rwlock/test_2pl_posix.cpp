#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 
#include <pthread.h>
#include <thread>

const int N = 100000;
int S[N]; //guarded by rwlock[N]
pthread_rwlock_t rwlock[N];

int sum_triple(const int i0, const int j){
    int err = 0;
    if(pthread_rwlock_trywrlock(&rwlock[j]) != 0){
        return -1; 
    }

    int d = 0;
    for(; d<3; ++d){
        int i = (i0+d)%N;
        if(i!=j){
            if(pthread_rwlock_tryrdlock(&rwlock[i]) != 0){
                err = -1;
                break;
            }
        }
    }

    if(err==0){
        S[j] = S[i0]+S[(i0+1)%N]+S[(i0+2)%N];
    }

    for(--d; d>=0; --d){
        int i = (i0+d)%N;
        if(i!=j){
            pthread_rwlock_unlock(&rwlock[i]);
        }
    }

    pthread_rwlock_unlock(&rwlock[j]);
    return err;
}

void *update_2pl(void *arg){
    srand((unsigned int)time(NULL));
    for(int n=0; n<10000; ++n){
        const int i = rand()%N;
        const int j = rand()%N;
        while(sum_triple(i, j) < 0){
            //retry because yield actively after deadlock
            std::this_thread::yield();
        }
    }
    return NULL;
}

void init_rwlock(){
    pthread_rwlockattr_t attr;
    pthread_rwlockattr_init(&attr);
    pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP); //avoid writer hungry
    //pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_READER_NP); //avoid reader hungry

    for(int i=0; i<N; ++i){
        pthread_rwlock_init(&rwlock[i], &attr);
    }
}

void destroy_rwlock(){
    for(int i=0; i<N; ++i){
        pthread_rwlock_destroy(&rwlock[i]);    
    }
}

int main(int argc,char** argv) {
    if(argc<2){
        fprintf(stderr, "Useage: ./test_2pl {{M}}\n");
        return -1;
    }

    const int M = atoi(argv[1]);
    fprintf(stderr, "M:%d\n", M);

    for(int i=0; i<N; ++i){
        S[i] = 1;
    }

    init_rwlock();

    pthread_t tid[M];    
    for(int i=0; i<M; ++i){
        pthread_create(&tid[i], NULL, update_2pl, NULL); 
    }
    for(int i=0; i<M; ++i){
        pthread_join(tid[i], NULL);
    }

    destroy_rwlock();

    for(int i=0; i<N; ++i){
        fprintf(stderr, "S[%d]:%d\n", i, S[i]);
    }
    return 0;
}
