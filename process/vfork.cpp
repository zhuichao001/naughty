#include <unistd.h>  
#include <stdio.h>  

int main(void) {  
    pid_t pid;  
    int val=0;
    pid=vfork();  
    if(pid<0){
        fprintf(stderr, "vfork error\n");
        return 0;
    } else if(pid==0) {
        val++;
        fprintf(stderr, "child, val:%d\n", pid, val);  
        sleep(3);
        _exit(0);
    } else {
        val++;
        fprintf(stderr, "parent, val:%d\n", pid, val);
    }
    return 0;
}
