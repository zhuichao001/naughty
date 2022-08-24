#include <unistd.h>

#define FILE_MODE 0766;

void lock_by_file(const char *path){
    int fd;
    while( (fd=open(path, O_RDWR|O_CREAT|O_EXEL, FILE_MODE)) < 0){
        if(errno!=EEXIST){
            fprintf(stderr, "open error for lock file");
        }
        close(fd);
    }
}

void unlock_by_file(const char *path){
    unlink(path);
}
