#include <stdio.h>
#include "high_precise.h"

void test_time(){
    double f =3.141592614;
    for(int i=0; i<1000000; ++i){
        f *= 1.012;
    }
}

int main(){
    unsigned long long start = rdtscp1();
    test_time();
    unsigned long long end = rdtscp2();
    fprintf(stderr, "cost:%d ns\n", end-start);
    return 0;
}
