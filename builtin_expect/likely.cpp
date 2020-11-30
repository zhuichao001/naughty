#include <stdio.h>
#include <stdlib.h>

#define likely(x)    __builtin_expect(!!(x), 1)
#define unlikely(x)  __builtin_expect(!!(x), 0)

int main(int argc, char *argv[]) {
    int x = atoi(argv[1]);
    if (unlikely(x == 2)) {
        ++x;
    } else {
        --x;
    }
    printf("%d\n", x);
    return 0;
}
