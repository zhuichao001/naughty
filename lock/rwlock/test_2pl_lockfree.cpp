#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 
#include <pthread.h>
#include <thread>
#include "rwlock.h"

const int N = 100000;
int S[N]; //guarded by rwlock[N]
rwlock_t rwlock[N];

int sum_triple(const int i0, const int j){
    int err = 0;
    if(rwlock[j].try_wlock() != 0){
        return -1;
    }

    int d = 0;
    for(; d<3; ++d){
        int i = (i0+d)%N;
        if(i!=j){
            if(rwlock[i].try_rlock() != 0){
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
            rwlock[i].runlock();
        }
    }

    rwlock[j].wunlock();
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

    pthread_t tid[M];    
    for(int i=0; i<M; ++i){
        pthread_create(&tid[i], NULL, update_2pl, NULL); 
    }
    for(int i=0; i<M; ++i){
        pthread_join(tid[i], NULL);
    }

    for(int i=0; i<N; ++i){
        fprintf(stderr, "S[%d]:%d\n", i, S[i]);
    }
    return 0;
}
