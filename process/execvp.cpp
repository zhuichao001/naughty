#include <unistd.h>
#include <stdio.h>
int main() {
    pid_t pid;

    if((pid=fork())<0) {
        perror("fork");
        return -1;
    } else if(pid==0) {
        printf("child process:my pid is %d\n",getpid());
        char *arg[]={"ls","-a","-l","/etc",NULL};
        if(execvp("ls",arg)<0) {
            perror("execv");
        }
    } else {
        printf("parent process:my pid is %d\n",getpid());
    }
    return 0;
}

