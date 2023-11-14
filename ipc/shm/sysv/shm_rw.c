#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define N 64

//System V API

typedef struct {
    pid_t pid;
    char buf[N];
} shm_data_t;

void sig_handler(int signo) {
    return;
}

int main() {
    key_t key;
    if ((key = ftok(".", 'm')) < 0) {
        perror("fail to ftok");
        exit(-1);
    }
    fprintf(stderr, "shm key:%d\n", key);

    signal(SIGUSR1, sig_handler);

    int shmid;
    if ((shmid = shmget(key, sizeof(shm_data_t), 0666|IPC_CREAT|IPC_EXCL)) < 0) {
        if (EEXIST == errno) {
            shmid = shmget(key, sizeof(shm_data_t), 0666);
            shm_data_t *p = (shm_data_t *)shmat(shmid, NULL, 0);
            fprintf(stderr, "%d read %d\n", getpid(), p->pid);

            pid_t pid = p->pid;
            p->pid = getpid();
            kill(pid, SIGUSR1);

            fprintf(stderr, "%d send %d\n", getpid(), getpid());
            shmdt(p);
            shmctl(shmid, IPC_RMID, NULL);

            return 0;
        } else {
            perror("fail to shmget");
            return -1;
        }
    } else {
        shm_data_t *p = (shm_data_t *)shmat(shmid, NULL, 0);
        p->pid = getpid();
        fprintf(stderr, "%d write %d\n", getpid(), p->pid);

        pause();

        fprintf(stderr, "%d recv %d\n", getpid(), p->pid);
        shmdt(p);
        shmctl(shmid, IPC_RMID, NULL);
        return 0;
    }
}
