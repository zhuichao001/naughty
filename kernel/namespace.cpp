/*
 Mount namespaces   CLONE_NEWNS
 UTS namespaces     CLONE_NEWUTS
 IPC namespaces     CLONE_NEWIPC
 PID namespaces     CLONE_NEWPID
 Network namespaces CLONE_NEWNET
 User namespaces    CLONE_NEWUSER
 */

#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>

const int STACK_SIZE = 8192;
char child_stack[8192];

int child(void* arg) {
    printf("Child - %d\n", getpid());
    return 1;
}

int main() {
    printf("Parent - fork child\n");
    int pid = clone(child, child_stack+8192, CLONE_NEWPID, NULL);
    if (pid == -1) {
        perror("clone error:");
        exit(1);
    }
    waitpid(pid, NULL, 0);
    printf("Parent - child(%d) exit\n", pid);
    return 0;
}
