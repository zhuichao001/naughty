#include <stdio.h>
#include <functional>


//std::bind and std::thread must use std::ref to enable pass by reference

void add(int a, int b, int &sum) {
    sum = a + b;
}

int main(){
    int a=3, b=5, sum=0;
    add(a, b, std::ref(sum));
    printf("sum of %d and %d is %d\n", a, b, sum);
    return 0;
}
