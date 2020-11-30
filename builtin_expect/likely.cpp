#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define likely(x)    __builtin_expect(!!(x), 1)
#define unlikely(x)  __builtin_expect(!!(x), 0)

int main(int argc, char *argv[]) {
    int a;
    a = atoi (argv[1]);
    if (unlikely (a == 2)) {
        ++a;
    } else {
        --a;
    }
    printf ("%d\n", a);
    return 0;
}

