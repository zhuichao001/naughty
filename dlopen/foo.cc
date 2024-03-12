#include <stdio.h>

extern "C" {

int bar(){
    printf("test bar.\n");
    return 0;
}

}
