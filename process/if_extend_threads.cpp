#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>

//conclusion: Will not extend sub threads from parent process

int gi = 0;

void *print(void *args){
    pid_t pid = (pid_t)syscall(SYS_gettid);
    while(1){
        printf("gi:%d, thread_id:%d\n", gi, pid);
        sleep(1);
    }
    return nullptr;
}

int main(){
    pthread_t ppid;
    pthread_create(&ppid, nullptr, print, nullptr);

    pid_t pid;
    if((pid = fork()) == 0){
        gi = 1; //sub process
    }

    char line[128];
    fgets(line, 128, stdin);
    return 0;
}
