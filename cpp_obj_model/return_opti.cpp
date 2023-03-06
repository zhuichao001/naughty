#include <stdio.h>

// Named Return Value（NRV）优化

class X{
public :
    X(){ printf("X() constructed.\n"); }
    X(int i):v(i){printf("X(int) constructed.\n"); }
    ~X(){ printf("X destroyed.\n");}
    int v = 0;
};


X f(){
    printf("enter f()\n");
    X x;
    x.v = 12;
    printf("return f()\n");
    return x;
}

/*
 * enter f()
 * X constructed.
 * return f()
 * finish f()
 * X destroyed.
 * */
void test1(){
    X x = f();
    printf("finish f()\n");
}

int main(){
    test1();
    printf("---------\n");
    test2();
    return 0;
}
