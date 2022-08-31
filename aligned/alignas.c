#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

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
