#include <stdio.h>

template <typename T>
void RefFunc(const T &a, const T &b){
    printf("RefFunc is called.\n");
}

template <typename T>
void NoRefFunc(T a, T b){
    printf("NoRefFunc is called.\n");
}

int main() {
    int *const ic = nullptr;
    const int *ci = nullptr;
    int *p = nullptr;

    RefFunc(p, ic);  // ok 顶层const可以被忽略 T 为 int *
    //RefFunc(p, ci);  // error 底层const不可以忽略
    //NoRefFunc(p, ci); // error 底层const不可以忽略

    int i = 0;
    int &ri = i;
    NoRefFunc(i, ri); // ok ri从int &转换为int

    int arr[4];
    NoRefFunc(p, arr);  // ok arr 被推断为int *

    NoRefFunc(4, 5.0);  // error T 可以推断为int或double
    return 0;
}
