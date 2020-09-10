#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


const int MAXLINE = 128;
const int STDOUT_FD = 1;

int pipeline() {
    int fd[2];
    if (pipe(fd) < 0) {
        perror("pipe error");
        return -1;
    }

    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork error");
        return -1;
    } else if(pid > 0) { //parent send
        close(fd[0]);

        const char *msg = "[message from parent]\n";
        write(fd[1], msg, strlen(msg));

        wait(pid);
        close(fd[1]);
    } else {
        close(fd[1]); //child receive

        char line[MAXLINE];
        int n = read(fd[0], line, MAXLINE);
        write(STDOUT_FD, line, n);

        close(fd[0]);
    }
    return 0;
}

int main(void) {
    int err = pipeline();
    exit(err);
}
