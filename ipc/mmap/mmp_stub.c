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
#include <string.h>

#define FILE_MODE 0X766
#define FILE_SIZE 8

struct shared_t {
    double val;
    int count;
}shared;

int main(int argc, char **argv){
    shared.val = 3.1415926;
    shared.count = 6;
    int fd = open("/dev/zero", O_RDWR, 0);
    //int fd = open("./zero", O_CREAT | O_RDWR, FILE_MODE);
    ftruncate(fd, FILE_SIZE);
    //write(fd, &shared, sizeof(shared));
    //close(fd);
    char *ptr = (char*)(struct shared_t *)mmap(NULL, FILE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    memcpy(ptr, "hello..", 8);

    pid_t pid;
    if((pid=fork())==0){ /*child*/
        char fdstr[10];
	memset(fdstr,0,10);
	sprintf(fdstr,"%d", fd);
	printf("before fd:%s\n", fdstr);
	if(execlp("python", "python", "worker.py", fdstr, (char *)0)){
		perror("failed");
	}
        //exit(0);
    }else{ /*parent*/
        waitpid(pid, NULL, 0);
        close(fd);
    }

    exit(0);
}
