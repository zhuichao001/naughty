#ifndef __NAUGHTY_COROUTINE_H__
#define __NAUGHTY_COROUTINE_H__

#include <ucontext.h>
#include <vector>

#define DEFAULT_STACK_SZIE (32<<10) //32k
#define MAX_ROUTINE_COUNT   1024

enum State{FREE, RUNNABLE, RUNNING, SUSPEND};

typedef void (*task_t)(void *arg);

typedef struct coroutine_t{
    ucontext_t ctx;
    task_t task;
    void *arg;
    enum State state;
    char stack[DEFAULT_STACK_SZIE];

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
            routines[i].state = FREE;
            routines[i].ctx.uc_stack.ss_sp = routines[i].stack;
            routines[i].ctx.uc_stack.ss_size = DEFAULT_STACK_SZIE;
            routines[i].ctx.uc_stack.ss_flags = 0;
        }
    }
    
    ~schedule_t() {
        delete []routines;
    }

    int getindex(enum State state){
        for(int i = 0; i < MAX_ROUTINE_COUNT; ++i ){
            if(routines[i].state == state){
                return i;
            }
        }
        return -1;
    }

    int create(task_t func){
        int idx = getindex(FREE);
        if(idx>=0 && routines[idx].state == FREE){
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
        cur_idx = getindex(RUNNABLE);
        if(cur_idx<0){
            return;
        }
        ucontext_t &sub = routines[cur_idx].ctx;
        swapcontext(&main, &sub);
    }

    void yield(){
        if(cur_idx != -1 ){
            coroutine_t *t = &routines[cur_idx];
            t->state = SUSPEND;
            cur_idx = -1;
            swapcontext(&(t->ctx), &main);
        }
    }

    void resume(){
        cur_idx = getindex(SUSPEND);
        if(cur_idx<0){
            return;
        }

        coroutine_t *t = &(routines[cur_idx]);
        if (t->state == SUSPEND) {
            swapcontext(&(main),&(t->ctx));
        }
    }

    bool finished(){
        return !(runnable() || resumable());
    }

    bool runnable(){
        return getindex(RUNNABLE)!=-1;
    }

    bool resumable(){
        return getindex(SUSPEND)!=-1;
    }

}schedule_t;

static void uthread_body(schedule_t *ps);

#endif
