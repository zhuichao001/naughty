#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <thread>

const char *kFileName = "./test.txt"; 

int test(){
    pthread_t tid = pthread_self();
    const int BUFFLEN = 4096;
    char c[BUFFLEN];
    sprintf(c, "pid:%ld write text data.", tid);
    memset(c+strlen(c), '.', BUFFLEN-strlen(c));
    c[BUFFLEN-2]='\n';
    c[BUFFLEN-1]=0;

    FILE *fp = fopen(kFileName, "a+");

    int lines=10000;
    while(--lines>0){
        fwrite(c, sizeof(char), strlen(c), fp);
        fflush(fp);
        fsync(fileno(fp));
    }

    fclose(fp);
    return 0;
}

int main () {
    std::thread t1(test);
    std::thread t2(test);
    t1.join();
    t2.join();
    return 0;
}

