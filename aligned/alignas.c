#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#ifndef CACHE_LINE_ALIGN
#define CACHE_LINE_SIZE 64
#define CACHE_LINE_ALIGN __attribute__((aligned(CACHE_LINE_SIZE)))
#endif

//aligned only supported by c++11 above
struct alignas(64) node_t{
    int *left;
    int *right;
    int val;
};

int main(){
    node_t a;
    char tmp[13];
    node_t b;
    fprintf(stderr,"addr of a:%d, b:%d\n", &a, &b);
    return 0;
};
