#include <stdio.h>

class B{
public:
    double fun(double f){printf("double B::fun(double) called.\n"); return 0.0;}
};

class D: public B{
public:
    int fun(double f, double g){printf("double D::fun(double) called.\n"); return 0;}
};


int main(){
    D *d = new D;
    int k = d->fun(3, 2);

    //double f = d->fun(3.0); // no matching function for call
    return 0;
}
