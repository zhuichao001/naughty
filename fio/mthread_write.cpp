#include <pthread.h>
#include <sys/types.h>
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
    pthread_t t1, t2;
    option_t opt1 = {.fp=fp, .data=TSTR};
    option_t opt2 = {.fp=fp, .data=OSTR};

    pthread_create(&t1, NULL, run_fwrite, &opt1);
    pthread_create(&t2, NULL, run_fwrite, &opt2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
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
    pthread_t t1, t2;

    option_t opt1 = {.fd=fd, .data=TSTR};
    option_t opt2 = {.fd=fd, .data=OSTR};

    pthread_create(&t1, NULL, run_write, &opt1);
    pthread_create(&t2, NULL, run_write, &opt2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    close(fd);
}

int main(){
    test_fwrite();
    test_write();
    return 0;
}
