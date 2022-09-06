#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/prctl.h>
 
void* run(void*arg) {
    char thread_name[16];
    prctl(PR_SET_NAME, (unsigned long)"test_psetname");
    prctl(PR_GET_NAME, (unsigned long)thread_name);
    printf("%s/n", thread_name);
    while(1){
        sleep(1);
    }
}
 
int main(void) {
    pthread_t tid;
    pthread_create(&tid, NULL, run, NULL);
    pthread_join(tid, NULL);
    return 0;
}
