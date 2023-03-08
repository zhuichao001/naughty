#include <stdio.h>

class B{
public:
    virtual int deal(int i){printf("int B::fun(int) called.\n"); return 0;}
};

class D: public B{
public:
    int deal(int i){printf("int D::fun(int) called.\n"); return 0;}
};


int main(){
    B *b = new D;
    int k = b->deal(3);

    double f = b->deal(3.0);
    return 0;
}
