#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

int system(const char *cmd){
    if(cmd==NULL){
        return -1;
    }

    pid_t pid;
    if((pid=fork()) < 0){
        return -1;
    }else if(pid==0){ //child
        execl("/bin/sh", "sh", "-c", cmd, (char*)0);
        _exit(127); /*execl error*/
    }else{
        while(waitpid(pid, NULL, 0)<0){
            if(errno!=EINTR){
                return -1;
            }
        }
    }
    return 0;
}
