#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>

static int func(void *arg) {
    char *buf = (char *)arg;
    strcpy(buf, "[child]\0");
    return 0;
}

int main(int argc, char **argv) {
    const int STACK_SIZE = 256 * 1024;
    char *stack = (char*)malloc(STACK_SIZE);

    int flags = 0;
    if (argc > 1 && !strcmp(argv[1], "clone_vm")) {
        flags |= CLONE_VM;
    }

    char buf[] = "[parent]";
    if (clone(func, stack + STACK_SIZE, flags, buf) == -1) {
        exit(1);
    }

    sleep(1);
    printf("In parent process, buf:\"%s\"\n", buf);
    return 0;
}
