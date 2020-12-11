#include <stdio.h>
#include "coroutine.h"

void func_a(void * arg) {
    fprintf(stderr, "func_a before coroutine yield\n");
    schedule_t * sch = (schedule_t *)arg;
    sch->await();
    fprintf(stderr, "func_a after coroutine yield\n");
}

void func_b(void * arg) {
    fprintf(stderr, "func_b before coroutine yield\n");
    schedule_t * sch = (schedule_t *)arg;
    sch->await();
    fprintf(stderr, "func_b after coroutine yield\n");
}

void coroutine_test() {
    schedule_t sch;
    sch.create(func_a);
    sch.create(func_b);
    while(!sch.finished()){
        sch.schedule();
    }
    fprintf(stderr, "test over\n");
}

int main() {
    coroutine_test();
    return 0;
}
