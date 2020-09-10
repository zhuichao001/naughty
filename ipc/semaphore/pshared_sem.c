#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>
#include <semaphore.h>


const int N_LOOP = 1600;


int main(int argc, char **argv) {
    const int ZERO = 0;
    int fd = open(".share", O_RDWR | O_CREAT, S_IRWXU);
    write(fd, &ZERO, sizeof(int)); 
    int* ptr = (int*)mmap((void*)NULL, sizeof(int), (PROT_READ | PROT_WRITE), MAP_SHARED, fd, 0);
    close(fd);
 
    sem_t* psem = (sem_t*)mmap((void*)NULL, sizeof(sem_t), (PROT_READ|PROT_WRITE), (MAP_ANON|MAP_SHARED), -1, 0);
    if (sem_init(psem, 1, 1) < 0) {
        perror("semaphore initilization");
        exit(0);
    }

    *ptr = 0;
    int pid;
    if ((pid=fork()) == 0) { //child
        for (int i = 0; i < N_LOOP; i++) {
            sem_wait(psem);
            fprintf(stderr, "<child>: %d\n", ++(*ptr));
            sem_post(psem);
        }
    } else {
        for (int i = 0; i < N_LOOP; i++) { //parent
            sem_wait(psem);
            fprintf(stderr, "<parent>: %d\n", ++(*ptr));
            sem_post(psem);
        }
        wait(pid);
        sem_close(psem);
    }
    exit(0);
}
