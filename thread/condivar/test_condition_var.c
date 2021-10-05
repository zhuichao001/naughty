#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "atomic/atomic.h"

typedef struct msg_t{
    int id;
    struct msg_t *next;
} msg_t;


struct msg_t *head;

pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

atomic_t ar = ATOMIC_INIT(0);
atomic_t as = ATOMIC_INIT(0);


msg_t *dequeue(){
    pthread_mutex_lock(&qlock);
    while(head==NULL){
        pthread_cond_wait(&qready, &qlock);
    }

    msg_t *msg = head;
    head = msg->next;
    pthread_mutex_unlock(&qlock);

    return msg;
}

void enqueue(msg_t *msg){
    pthread_mutex_lock(&qlock);
    msg->next = head;
    head = msg;
    pthread_mutex_unlock(&qlock);
    pthread_cond_signal(&qready);
}

void *produce(void* arg){
    for(int i=0; i<10000; ++i){
        msg_t *msg = malloc(sizeof(msg_t));
        msg->id = atomic_add_return(1, &as);
        enqueue(msg);
    }
}

void *consume(void* arg){
    for(;;){
        msg_t *msg = dequeue();
        fprintf(stderr, "msg id: %d\n", msg->id);
        free(msg);
        atomic_inc(&ar);
    }
}

void test(){
    pthread_t consumers[4];
    pthread_t producers[4];
    for(int i=0; i<4; ++i){
        pthread_create(&consumers[i], NULL, consume, NULL);
        pthread_create(&producers[i], NULL, produce, NULL);
    }

    for(int i=0; i<4; ++i){
        pthread_join(producers[i], NULL);
    }

    while(1){
        if(atomic_read(&as) != atomic_read(&ar)){
            sleep(1);
            continue;
        }

        for(int i=0; i<4; ++i){
            pthread_cancel(consumers[i]);
        }
        break;
    }
}

int main(){
    test();
    return 0;
}
