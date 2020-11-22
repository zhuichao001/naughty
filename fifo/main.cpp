#include <stdio.h>
#include "ufifo.h"


int main(){
    ufifo u(7);
    char h[8]={0,};

    u.in((const uint8_t*)"1234567", 7);
    int len = u.out((uint8_t*)h, 8);
    printf("out len=%d, data=%s\n", len, h);
}
