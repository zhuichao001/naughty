#include <unistd.h>  
#include <stdio.h>  

int gval = 9;

int main(void) {  
    int val=100;
    pid_t pid=vfork();  
    if(pid<0){
        fprintf(stderr, "vfork error\n");
        return 0;
    } else if(pid==0) {
        ++val;
        ++gval;
        fprintf(stderr, "child, val:%d, global val:%d\n", val, gval);  
        sleep(3);
        _exit(0); //will dead-clock if missed exit() or exec()
    } else {
        fprintf(stderr, "parent, val:%d, global val:%d\n", val, gval);
    }
    return 0;
}
