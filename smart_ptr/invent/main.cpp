//#include <memory>
#include <stdio.h>
#include "smart_ptr.h"


void test0(){ //normal case
    shared_ptr<int> m;
    {
        shared_ptr<int> p(new int);
        *p = 123;
        printf("val:%d, use_count:%d\n", *p, p.use_count());

        m = p;
        printf("val:%d, use_count:%d\n", *m, m.use_count());
    }
    printf("val:%d, use_count:%d\n", *m, m.use_count());
}

void test1(){ //wrong case
    int *a = new int();
    printf("a: %p\n", a);

    shared_ptr<int> p1(a);
    shared_ptr<int> p2(a);
}

void test2(){ //wrong case
    int *a = new int;
    shared_ptr<int> p1(a);
    shared_ptr<int> p2(p1.get());
}

int main(){
    test1();
    return 0;
}
