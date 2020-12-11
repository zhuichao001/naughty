#include <stdio.h>
#include "coroutine.h"


void func_a(void * arg) {
    printf("func_a before coroutine yield\n");
    schedule_t * sch = (schedule_t *)arg;
    sch->yield();
    printf("func_a after coroutine yield\n");
}

void func_b(void * arg) {
    printf("func_b before coroutine yield\n");
    schedule_t * sch = (schedule_t *)arg;
    sch->yield();
    printf("func_b after coroutine yield\n");
}

void coroutine_test() {
    schedule_t sch;
    sch.create(func_a);
    sch.create(func_b);
    while(!sch.finished()){
        while(sch.runnable()){
            sch.schedule();
        }
        while(sch.resumable()){
            sch.resume();
        }
    }
    printf("test over\n");
}

int main() {
    coroutine_test();
    return 0;
}
