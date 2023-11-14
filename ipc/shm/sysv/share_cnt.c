#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NLOOP 65536
#define FILE_MODE 0X766

struct shared_t {
    sem_t mutex;
    int count;
}shared;

int main(int argc, char **argv){
    int fd = open("./tmp.share_cnt", O_RDWR|O_CREAT, FILE_MODE);
    write(fd, &shared, sizeof(shared));
    struct shared_t *ptr = (struct shared_t *)mmap(NULL, sizeof(shared), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    sem_init(&ptr->mutex, 1, 1);

    pid_t childfd;
    if((childfd=fork())==0){ /*child*/
        for(int i=0; i<NLOOP; ++i){
            sem_wait(&ptr->mutex);
            fprintf(stderr, "child: %d\n", ++ptr->count);
            sem_post(&ptr->mutex);
            usleep(1);
        }
        exit(0);
    }

    for(int i=0; i<NLOOP; ++i){ /*parent*/
        sem_wait(&ptr->mutex);
        fprintf(stderr, "parent: %d\n", ++ptr->count);
        sem_post(&ptr->mutex);
        usleep(1);
    }

    waitpid(childfd, NULL, 0);
    exit(0);
}
