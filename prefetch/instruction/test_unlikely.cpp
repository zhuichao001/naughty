#include<stdio.h>
#include<stdlib.h>

#define likely(x) __builtin_expect(!!(x), 1) //gcc built-in function
#define unlikely(x) __builtin_expect(!!(x), 0)

int main(int argc, char* argv[]){
    if (argc<2) {
        fprintf(stderr, "usage: ./test_unlikely <val>\n");
        exit(-1);
    }

    int x = 0;
    x = atoi(argv[1]);

    if (unlikely(x == 0)) {
        x = x + 10;
    } else {
        x = x + 100;
    }

    printf("result x=%d\n", x);
    return 0;
}
