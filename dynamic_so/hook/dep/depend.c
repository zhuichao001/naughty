#include <stdlib.h>
#include <stdio.h>

void display() {
    char *buf = malloc(1024);
    if(NULL != buf) {
        snprintf(buf, 1024, "%s", "[libdepend.so] display...\n");
        printf("%s", buf);
    }
}
