#include <stdio.h>

class X{
public :
    X(){ printf("X constructed.\n"); }
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
int main(){
    X x = f();
    printf("finish f()\n");
    return 0;
}
