#pragma once

typedef struct rwlock_t{
    //0:no readers and writers; positive: n readers shared; -1: writer occupied
    std::atomic<int> rwcnt; 

    rwlock_t():
        rwcnt(0){
    }

    int try_rlock(){
        int previous;
        while((previous = rwcnt.load(std::memory_acquire))>=0){
            if(rwcnt.compare_exchange_strong(previous, previous+1, std::memory_order_acq_rel)){
                return 0;
            }
        }
        return -1;
    }

    void rlock(){
        while(rwcnt.load(std::memory_order_acquire)==-1){
            asm volatile("pause" : : : "memory");
        }
        ++rwcnt;
    }

    void runlock(){
        std::atomic_thread_fence(std::memory_order_release);
        --rwcnt;
    }

    int try_wlock(){
        if(rwcnt.load(std::memory_acquire)!=0){
            return -1;
        }
        return 0;
    }

    void wlock(){
        while(rwcnt.compare_exchange_strong(0, -1, std::memory_order_acq_rel)){
            asm volatile("pause" : : : "memory");
        }
    }

    void wunlock(){
        rwcnt.store(0, std::memory_order_release);
    }
}rwlock_t;
