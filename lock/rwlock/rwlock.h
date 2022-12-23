#pragma once

#include<atomic>
#include<memory>

typedef struct rwlock_t{
    //0:no readers or writers; positive n: n readers shared; -1: one writer occupied
    std::atomic<int> rwcnt; 

    rwlock_t():
        rwcnt(0){
    }

    int try_rlock(){
        int previous;
        while((previous = rwcnt.load(std::memory_order_acquire))>=0){
            if(rwcnt.compare_exchange_strong(previous, previous+1, std::memory_order_acq_rel)){
                return 0;
            }
        }
        return -1;
    }

    void rlock(){
        while(true){
            int previous = rwcnt.load(std::memory_order_acquire);
            if(previous<0){
                asm volatile("pause" : : : "memory");
                continue;
            }
            if(rwcnt.compare_exchange_strong(previous, previous+1, std::memory_order_acq_rel)){
                return;
            }
        }
    }

    void runlock(){
        --rwcnt;
        std::atomic_thread_fence(std::memory_order_release);
    }

    int try_wlock(){
        if(rwcnt.load(std::memory_order_acquire)!=0){
            return -1;
        }
        int writers = 0;
        if(rwcnt.compare_exchange_strong(writers, -1, std::memory_order_acq_rel)){
            return 0;
        }
        return -1;
    }

    void wlock(){
        int none = 0;
        while(rwcnt.compare_exchange_strong(none, -1, std::memory_order_acq_rel)){
            asm volatile("pause" : : : "memory");
        }
    }

    void wunlock(){
        rwcnt.store(0, std::memory_order_release);
    }
}rwlock_t;
