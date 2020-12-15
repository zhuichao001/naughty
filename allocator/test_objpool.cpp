#include <stdio.h>
#include "objpool.h"

class A{
public:
    int ival;
    float fval;
    char data[35];
};

int main(){
    naughty::objpool<A> pool;
    A *a = pool.fetch();
    a->ival=1;
    a->fval=3.14;
    const char *str = "test allocator";
    memcpy(a->data, str, strlen(str));
    pool.putin(a);
}
