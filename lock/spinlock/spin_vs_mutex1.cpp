#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/syscall.h>

#define USE_SPINLOCK 1

#ifdef USE_SPINLOCK
pthread_spinlock_t spinlock;
#else
pthread_mutex_t mutex;
#endif

__uint64_t g_count;

pid_t gettid() {
    return syscall(SYS_gettid);
}

void *run(void *arg) {
    fprintf(stderr, "Thread %lu started\n", (unsigned long)gettid());

    for(int i = 0; i < 1000; i++){
#ifdef USE_SPINLOCK
        pthread_spin_lock(&spinlock);
#else
        pthread_mutex_lock(&mutex);
#endif
        for(int j = 0; j < 1000000; j++){
            g_count++;
        }
#ifdef USE_SPINLOCK
        pthread_spin_unlock(&spinlock);
#else
        pthread_mutex_unlock(&mutex);
#endif
    }

    fprintf(stderr, "Thread %lu finished\n", (unsigned long)gettid());
    return NULL;
}

int main(int argc, char *argv[]){
#ifdef USE_SPINLOCK
    pthread_spin_init(&spinlock, 0);
#else
    pthread_mutex_init(&mutex, NULL);
#endif

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
