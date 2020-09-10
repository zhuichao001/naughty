#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

typedef struct context_t {
    int count;
    sem_t sem;
}context_t;

void* fun_add(void *arg) {
    context_t *data = (context_t*)arg;
    sem_wait(&data->sem);

    ++data->count;
    printf("add thread<%ld> : count = %d\n", pthread_self(), data->count);

    sem_post(&data->sem);
    return NULL;
}

void* fun_minus(void *arg) {
    context_t *data = (context_t*)arg;
    sem_wait(&data->sem);

    --data->count;
    printf("minus thread<%ld> : count = %d\n", pthread_self(), data->count);

    sem_post(&data->sem);
    return NULL;
}

int main(int argc, char *argv[]) {
    context_t data;
    data.count = 1;

    int pshared=1, val=1;
    sem_init(&data.sem, pshared, val);

    pthread_t ta, tb;
    pthread_create(&tb, NULL, fun_minus, &data);
    pthread_create(&ta, NULL, fun_add, &data);

    pthread_join(ta, NULL);
    pthread_join(tb, NULL);
    return 0;
}
