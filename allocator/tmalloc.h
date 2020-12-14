#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>
#include <memory.h>

namespace naughty{

enum {STEP_BYTES = 16};
enum {MAX_BYTES = 256};
enum {NUM_LISTS = MAX_BYTES/STEP_BYTES};

size_t ROUND_UP(size_t bytes) {
    return (bytes + STEP_BYTES-1) & ~(STEP_BYTES-1);
}

size_t FREELIST_INDEX(size_t bytes) {
    return (bytes + STEP_BYTES-1) / STEP_BYTES - 1; 
}

typedef struct trunk_t {
    union {
        trunk_t* next;
        long long fidx; //free index
    };
    char data[1];
}trunk_t;


template<class T>
class tmalloc {
public:
    typedef T   value_type;
    typedef T*  pointer;
    typedef const T* const_pointer;
    typedef T&  reference;
    typedef const T& const_reference;
    typedef size_t  size_type;
    typedef ptrdiff_t difference_type;

    tmalloc(){
        for(int fidx=0; fidx<NUM_LISTS; ++fidx){
            refill(fidx);
        }
    }

    ~tmalloc(){
        //TODO FREE
    }

    pointer allocate(size_type n) {
        if(n > (size_t)MAX_BYTES){
            return pointer(malloc(n));
        }

        const int fidx = FREELIST_INDEX(n);
        trunk_t ** cur_list = &free_list[fidx];
        trunk_t * result = *cur_list;
        if(result==NULL){
            refill(fidx);  //TODO
            return allocate(n);
        }
        *cur_list = result->next;
        return (pointer)(result->data);
    }

    void deallocate(pointer p) {
        trunk_t * obj = (trunk_t*)((void*)p - 8);
        obj->next = free_list[obj->fidx];
        free_list[obj->fidx]->next = obj;
    }

    void construct(pointer p, const T& value) {
        new((T*)p) T(value);
    }

    void destroy(pointer p) {
        ((T*)p)->~T();
    }

private:
    trunk_t* free_list[NUM_LISTS];

    void refill(const int fidx){
        const int HUGE_SPACE_SIZE = 32<<10;
        const int TRUNK_SIZE = (fidx+1)*STEP_BYTES;

        free_list[fidx] = (trunk_t*)malloc(HUGE_SPACE_SIZE);  //TODO
        memset(free_list[fidx], 0, HUGE_SPACE_SIZE);

        trunk_t *inner = free_list[fidx];
        for(int j=0; j<HUGE_SPACE_SIZE/TRUNK_SIZE-1; ++j){
            inner->next = (trunk_t*)((void*)free_list[fidx] + TRUNK_SIZE);
            inner = (trunk_t*)((void*)inner+TRUNK_SIZE);
        }
    }

    pointer address(reference x) {
        return (pointer)&x;
    }

    const_pointer const_address(const_reference x) {
        return (const_pointer)&x;
    }
};

}//end namespace naughty

#endif
