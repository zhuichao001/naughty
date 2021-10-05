#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/time.h>
#include <list>
#include <pthread.h>

std::list<int> queue;

#ifdef USE_SPINLOCK
pthread_spinlock_t spinlock;
#else
pthread_mutex_t mutex;
#endif

pid_t gettid(){
    return syscall( __NR_gettid );
}

void *run(void *arg){
    fprintf(stderr, "Thread %lu started\n", gettid());

    while (1){
#ifdef USE_SPINLOCK
        pthread_spin_lock(&spinlock);
#else
        pthread_mutex_lock(&mutex);
#endif

        if (queue.empty()){
#ifdef USE_SPINLOCK
            pthread_spin_unlock(&spinlock);
#else
            pthread_mutex_unlock(&mutex);
#endif
            break;
        }

        int v = queue.front();
        queue.pop_front();

#ifdef USE_SPINLOCK
        pthread_spin_unlock(&spinlock);
#else
        pthread_mutex_unlock(&mutex);
#endif
    }

    fprintf(stderr, "Thread %lu finished\n", (unsigned long)gettid());
    return NULL;
}

int main(){
#ifdef USE_SPINLOCK
    pthread_spin_init(&spinlock, 0);
#else
    pthread_mutex_init(&mutex, NULL);
#endif

    for(int i = 0; i < 40000000; i++){
        queue.push_back(i);
    }

    const int THREAD_NUM = 2;
    pthread_t threads[THREAD_NUM];

    for(int i = 0; i < THREAD_NUM; i++){
        pthread_create(&threads[i], NULL, run, NULL);
    }

    for(int i = 0; i < THREAD_NUM; i++){
        pthread_join(threads[i], NULL);
    }

#ifdef USE_SPINLOCK
    pthread_spin_destroy(&spinlock);
#else
    pthread_mutex_destroy(&mutex);
#endif

    return 0;
}
