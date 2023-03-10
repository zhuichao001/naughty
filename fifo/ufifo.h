#ifndef _USR_FIFO_H_
#define _USR_FIFO_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <memory.h>
#include <errno.h>
#include <assert.h>

#if defined(__GNUC__) || defined(__x86_64__)
#define COMPILER_BARRIER() __asm__ __volatile("" : : : "memory")
#define MEMORY_BARRIER() __asm__ __volatile__("mfence": : : "memory")
#define smp_mb() MEMORY_BARRIER()
#define smp_rmb() COMPILER_BARRIER()
#define smp_wmb() COMPILER_BARRIER()  
#endif

#define Min(x,y) ((x)<(y)?(x):(y))

uint32_t roundup_pow_of_two(uint32_t v) {
    --v;
    for(int i=1; i<=16; i<<=1){
        v |= v >> 1;
    }
    return ++v;
}


class ufifo{
    uint32_t size; //2的整幂对齐
    uint8_t *data;
    uint32_t ridx;
    uint32_t widx;
public:
    ufifo(uint32_t capacity){
        size = roundup_pow_of_two(capacity);
        data = (uint8_t *)malloc(size);
        assert(data!=nullptr);

        printf("align %d is size:%d\n", capacity, size);

        ridx = 0;
        widx = 0;
    }

    uint32_t used()const{
        return ((widx+size)-ridx) & (size-1);
    }

    uint32_t unused()const{
        return size - used() - 1;
    }

    uint32_t push(const uint8_t *src, uint32_t limit){
        uint32_t _unused = unused(); 
        uint32_t len = Min(limit, _unused);
        smp_mb();
        uint32_t right = Min(widx+len, size);
        uint32_t first = right-widx;
        memcpy(data+widx, src, first);
        memcpy(data, src+first, len-first);
        smp_wmb();
        widx = (widx+len) & (size-1);
        return len;
    }

    uint32_t pop(uint8_t *dst, uint32_t limit){
        uint32_t _used = used();
        uint32_t len = Min(limit, _used);
        uint32_t right = Min(ridx+len, size);
        uint32_t first = right-ridx;
        smp_rmb();
        memcpy(dst, data+ridx, first);
        memcpy(dst+first, data, len-first);
        smp_mb();
        ridx = (ridx+len) & (size-1);
        return len;
    }

    ~ufifo(){
        if(data!=nullptr){
            free(data);
            data = nullptr;
        }
    }
};


#endif
