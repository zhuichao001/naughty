#include <stdio.h>
#include "tmalloc.h"


class A{
public:
    int a;
    double b;
    char c[35];
};


int main(){
    naughty::tmalloc<A> ta;
    A *a = ta.allocate(sizeof(A));
}
