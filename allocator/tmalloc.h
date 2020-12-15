#ifndef _NAUGHTY_TMALLOC_H_
#define _NAUGHTY_TMALLOC_H_

#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>
#include <vector>
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


class tmalloc {
public:
    tmalloc() {
        for(int fidx=0; fidx<NUM_LISTS; ++fidx){
            refill(fidx);
        }
    }

    ~tmalloc() {
        for(int i=0; i<huge_pages.size(); ++i){
            delete []huge_pages[i];
        }
    }

    void * alloc(size_t n) {
        if(n > (size_t)MAX_BYTES){
            return malloc(n);
        }
        const int fidx = FREELIST_INDEX(n);
        trunk_t * head = free_list[fidx];
        if(head==nullptr){
            refill(fidx);
            if(free_list[fidx]==nullptr){
                return nullptr;
            }
        }
        free_list[fidx] = head->next;
        head->fidx = fidx;
        return (void*)(head->data);
    }

    void dealloc(void *p) {
        trunk_t * t = (trunk_t*)((void*)p - sizeof(long long));
        int fidx = t->fidx;
        t->next = free_list[t->fidx];
        free_list[fidx] = t;
    }

private:
    trunk_t* free_list[NUM_LISTS];
    std::vector<void *> huge_pages;

    void refill(const int fidx) {
        const int HUGE_SPACE_SIZE = 32<<10;
        const int trunk_size = (fidx+1)*STEP_BYTES;
        void *page = malloc(HUGE_SPACE_SIZE);
        if(page==nullptr){
            fprintf(stderr, "ERROR:OUT OF MEMORY.\n");
            return;
        }
        memset(page, 0, HUGE_SPACE_SIZE);
        free_list[fidx] = (trunk_t*)page;
        huge_pages.push_back(page);
        //link trunc list
        trunk_t *inner = free_list[fidx];
        for(int j=0; j<HUGE_SPACE_SIZE/trunk_size-1; ++j){
            inner->next = (trunk_t*)((void*)inner + trunk_size);
            inner = inner->next;
        }
    }
};

}//end namespace naughty

#endif
