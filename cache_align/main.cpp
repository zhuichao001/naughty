#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define CACHE_ALIGN_64 __attribute__((__aligned__(64)))
#define CACHE_ALIGN_32 __attribute__((__aligned__(32)))
#define CACHE_ALIGN_16 __attribute__((__aligned__(16)))

typedef CACHE_ALIGN_64 struct D {
    int va;
    double fu;
    int vb;
    int vc;
} D;

typedef CACHE_ALIGN_16 struct E {
    char va;
    D md;
    int vb;
} E;

int main() {
    cout << "size of D:" << sizeof(D) << endl;
    cout << "size of E:" << sizeof(E) << endl;
    return 0;
}
