#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main() {
    const char *SEM_NAME = ".named_sem";
    sem_t *mutex = sem_open(SEM_NAME, O_CREAT, 0644, 1);
    if(mutex == SEM_FAILED) {
        perror("unable to create semaphore");
        sem_unlink(SEM_NAME);
        exit(-1);
    }
    
    key_t   key = 117;
    const int SHM_SIZE = 1+26;
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if(shmid < 0) {
        perror("failure in shmget");
        exit(-1);
    }

    char *s = (char*)shmat(shmid, NULL, 0);
    for(int i=1; i<=26; ++i) {
        sem_wait(mutex);
        *(s+i) = 'A'+i-1;
        sem_post(mutex);
    }

    while(*s != '*') {
        sleep(1);
    }

    sem_close(mutex);
    sem_unlink(SEM_NAME);
    shmctl(shmid, IPC_RMID, 0);
    return 0;
}
