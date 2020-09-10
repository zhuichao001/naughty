#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(){
    const char *SEM_NAME = ".named_sem";
    sem_t *mutex = sem_open(SEM_NAME, 0, 0644, 0);
    if(mutex == SEM_FAILED){
        perror("reader:unable to execute semaphore");
        sem_close(mutex);
        exit(-1);
    }

    key_t IPC_KEY = 0x319384;
    const int SHM_SIZE = 1+26;
    int shmid = shmget(IPC_KEY, SHM_SIZE, 0666);
    if(shmid < 0){
        perror("failure in shmget");
        exit(-1);
    }

    char *s = (char*)shmat(shmid, NULL, 0);
    for(int i=1; i<=26; ++i) {
        sem_wait(mutex);
        putchar(*(s+i));
        putchar('\n');
        sem_post(mutex);
    }

    *s = '*'; //end notation
    sem_close(mutex);
    shmctl(shmid, IPC_RMID, 0);

    return 0;
}
