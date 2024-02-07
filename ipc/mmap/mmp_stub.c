#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#define FILE_MODE 0X766
#define FILE_SIZE 8

int dev0_fd(){
    int fd = open("/dev/zero", O_RDWR, 0);
    return fd;
}

int file_fd(){
    int fd = open("./zero", O_CREAT | O_RDWR, FILE_MODE);
    return fd;
}

int shm_fd(){
    const char *path = "tmp.shm_create";
    int flags = O_RDWR | O_CREAT;
    int fd = shm_open(path, flags, FILE_MODE);
    return fd;
}

int main(int argc, char **argv){
    int fd = file_fd();
    ftruncate(fd, FILE_SIZE);
    char *ptr = (char*)(struct shared_t *)mmap(NULL, FILE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    memcpy(ptr, "hello..", 8);

    ftruncate(fd, FILE_SIZE+FILE_SIZE);
    char *ptr2 = (char*)(struct shared_t *)mmap(NULL, FILE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, FILE_SIZE);
    memcpy(ptr2, "world..", 8);

    //pid_t pid;
    //if((pid=fork())==0){ /*child*/
    //    char fdstr[10];
    //memset(fdstr,0,10);
    //sprintf(fdstr,"%d", fd);
    //printf("before fd:%s\n", fdstr);
    //if(execlp("python", "python", "worker.py", fdstr, (char *)0)){
    //    perror("failed");
    //}
    //    //exit(0);
    //}else{ /*parent*/
    //    waitpid(pid, NULL, 0);
    //    close(fd);
    //}

    exit(0);
}
