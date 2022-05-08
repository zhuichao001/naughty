#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

const int LOOP_COUNT = 5000000;
const char *OSTR = "OOOOOOOOO\n";
const char *TSTR = "TTTTTTTTT\n";

typedef struct {
    union{
        FILE *fp;
        int fd;
    };
    const char *data;
}option_t;

void * run_fwrite(void *arg){
    option_t *opt = (option_t *)arg;
    
    size_t len = strlen(opt->data);
    for(int i=0; i< LOOP_COUNT; ++i){
        fwrite(opt->data, len, 1, opt->fp);
    }
    return NULL;
}

void test_fwrite(){
    FILE *fp = fopen("fwrite.txt", "a+");

    pid_t pid;
    if((pid=fork())<0){
        fprintf(stderr, "fork error\n");
    }else if(pid==0){
        option_t opt1 = {.fp=fp, .data=TSTR};
        run_fwrite(&opt1);
    }else{
        option_t opt2 = {.fp=fp, .data=OSTR};
        run_fwrite(&opt2);
        wait(NULL);
    }

    fclose(fp);
}

void * run_write(void *arg){
    option_t *opt = (option_t *)arg;
    
    size_t len = strlen(opt->data);
    for(int i=0; i< LOOP_COUNT; ++i){
        write(opt->fd, opt->data, len);
    }
    return NULL;
}

void test_write(){
    int fd = open("write.txt", O_CREAT|O_WRONLY|O_TRUNC|O_APPEND);

    pid_t pid;
    if((pid=fork())<0){
        fprintf(stderr, "fork error\n");
    }else if(pid==0){
        option_t opt1 = {.fd=fd, .data=TSTR};
        run_write(&opt1);
    }else{
        option_t opt2 = {.fd=fd, .data=OSTR};
        run_write(&opt2);
        wait(NULL);
    }

    close(fd);
}

int main(){
    test_fwrite();
    test_write();
    return 0;
}
