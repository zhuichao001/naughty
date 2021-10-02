#include <stdio.h>
#include "shared.h"

int gc = 0;
void display_so(){
    ++gc;
    fprintf(stderr, "gc:%d\n", gc);
}
