#include <chrono>
#include <thread>
#include <random>
#include <stdio.h>
#include <pthread.h>
#include "thread_pool.h"


int func(int duration){
    long long tid = pthread_self();
    fprintf(stderr, "[tid:%lld]  hello, func !\n", tid);
    fprintf(stderr, "[tid:%lld]  ======= func sleep %d  =========  \n", tid, duration);
    std::this_thread::sleep_for(std::chrono::milliseconds(duration));
    return duration;
}


int main(){
    long long tid = pthread_self();
    thread_pool_t pool(8);
    auto cb = pool.post(func, 3);
    auto ret = cb.get();
    return 0;
}
