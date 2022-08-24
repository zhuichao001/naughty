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

const char *path_a = "./tmp.addr";
const char *path_b = "/etc/motd";

#define FILE_MODE 0X766

int main(){
    shm_unlink(path_a);

    int fd1 = shm_open(path_a, O_RDWR|O_CREAT|O_EXCL, FILE_MODE);
    ftruncate(fd1, sizeof(int));

    int fd2 = open(path_b, O_RDONLY);
    struct stat st;
    fstat(fd2, &st);

    int *ptr1, *ptr2;
    pid_t childpid;
    if((childpid=fork())){
        ptr2 = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd2, 0);
        ptr1 = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd1, 0);
        fprintf(stderr, "child: shm ptr=%p, motd ptr=%p\n", ptr1, ptr2);

        sleep(1);
        fprintf(stderr, "shared memory integer=%d\n", *ptr1);

        _exit(0);
    }

    ptr1 = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd1, 0);
    ptr2 = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd2, 0);
    fprintf(stderr, "parent: shm ptr=%p, motd ptr=%p\n", ptr1, ptr2);

    *ptr1 = 999;

    waitpid(childpid, NULL, 0);
    _exit(0);
}
