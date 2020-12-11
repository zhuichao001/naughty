#include <ucontext.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

void imsubtask() {
    fprintf(stderr, "imsubtask running...\n");
    sleep(2);
    fprintf(stderr, "imsubtask finished.\n");
}


void runCroutine(void(*func)(void)){
    char stack[4<<10];
    ucontext_t root;
    getcontext(&root);

    ucontext_t branch = root;
    {
        branch.uc_stack.ss_sp = stack;
        branch.uc_stack.ss_size = sizeof(stack);
        branch.uc_stack.ss_flags = 0;
        branch.uc_link = &root;
    }

    makecontext(&branch, (void(*)(void))func, 0);
    swapcontext(&root, &branch);
}

int main() {
    runCroutine(imsubtask);
    return 0;
}
