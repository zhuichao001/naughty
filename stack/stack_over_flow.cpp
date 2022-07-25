#include <stdio.h>
#include <stdlib.h>

#define BUFFER_LEN 24

void bad() {
    printf("Haha, I am hacked.\n");
    exit(1);
}

void copy(char* dst, char* src, int n) {
    for (int i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}

void test(char* t, int n) {
    char s[16];
    copy(s, t, n);
}

int main() {
    char t[BUFFER_LEN] = {
        'w', 'o', 'l', 'd',
        'a', 'b', 'a', 'b', 'a', 'b',
        'a', 'b', 'a', 'b', 'a', 'b',
    };

    int n = BUFFER_LEN - 8;
    int i = 0;
    for (; i < 8; i++) {
        t[n+i] = (char)((((long)(&bad)) >> (i*8)) & 0xff);
    }

    test(t, BUFFER_LEN);
    printf("hello\n");
}
