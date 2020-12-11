#ifndef __NAUGHTY_COROUTINE_H__
#define __NAUGHTY_COROUTINE_H__

#include <ucontext.h>
#include <vector>

const int DEFAULT_STACK_SZIE = (32<<10); //32k
const int MAX_ROUTINE_COUNT  = 8;
const int FREE=1, RUNNABLE=2, RUNNING=4, SUSPEND=8;

typedef void (*task_t)(void *arg);

typedef struct coroutine_t{
    ucontext_t ctx;
    task_t task;
    void *arg;
    int state;
    char stack[DEFAULT_STACK_SZIE];

    void init(){
        state = FREE;
        ctx.uc_stack.ss_sp = stack;
        ctx.uc_stack.ss_size = DEFAULT_STACK_SZIE;
        ctx.uc_stack.ss_flags = 0;
    }

}coroutine_t;

void digest(coroutine_t *r){
    r->state = RUNNING;
    r->task(r->arg);
    r->state = FREE;
}

typedef struct schedule_t{
    ucontext_t main;
    coroutine_t *routines;
    int cur_idx;

    schedule_t(){
        cur_idx = -1;
        routines = new coroutine_t[MAX_ROUTINE_COUNT];
        for (int i = 0; i < MAX_ROUTINE_COUNT; i++) {
            routines[i].init();
        }
    }
    
    ~schedule_t() {
        delete []routines;
    }

    int getindex(int states){
        for(int i = 0; i < MAX_ROUTINE_COUNT; ++i ){
            if((routines[i].state & states) == routines[i].state){
                return i;
            }
        }
        return -1;
    }

    int create(task_t func){
        int idx = getindex(FREE);
        if(idx<0){
            return -1;
        }
        if(routines[idx].state == FREE){
            routines[idx].task = func;
            routines[idx].arg = this;
            routines[idx].state = RUNNABLE;
            coroutine_t &sub = routines[idx];
            getcontext(&sub.ctx);
            sub.ctx.uc_link = &(main);
            makecontext(&sub.ctx, (void(*)(void))(digest), 1, &sub);
        }
        return idx;
    }

    void schedule(){
        cur_idx = getindex(RUNNABLE|SUSPEND);
        if(cur_idx<0){
            return;
        }
        ucontext_t &sub = routines[cur_idx].ctx;
        swapcontext(&main, &sub);
    }

    void await(){
        if(cur_idx<0){
            return;
        }
        coroutine_t *t = &routines[cur_idx];
        cur_idx = -1;
        t->state = SUSPEND;
        swapcontext(&(t->ctx), &main);
    }

    bool finished(){
        return getindex(RUNNABLE|SUSPEND) == -1;
    }

}schedule_t;

#endif
