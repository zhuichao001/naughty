#include <stdio.h>
#include "tallocator.h"


class A{
public:
    int ival;
    float fval;
    char data[35];
};


int main(){
    naughty::tallocator<A> ta;
    A *a = ta.alloc();
    a->ival=1;
    a->fval=3.14;
    const char *str = "test allocator";
    memcpy(a->data, str, strlen(str));
    ta.dealloc(a);
}
