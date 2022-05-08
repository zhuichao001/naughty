#include <stdio.h>
#include <sys/stat.h>

int test_stat(){
    struct stat st;
    if(lstat("./test_stat.cpp", &st)<0){
        fprintf(stderr, "lstat(2) error\n");
        return -1;
    }

    const char *type;
    if(S_ISREG(st.st_mode)){
        type = "regular";
    }else if(S_ISDIR(st.st_mode)){
        type = "directory";
    }else if(S_ISCHR(st.st_mode)){
        type = "character special";
    }else if(S_ISBLK(st.st_mode)){
        type = "block special";
    }else if(S_ISFIFO(st.st_mode)){
        type = "fifo";
    }else if(S_ISLNK(st.st_mode)){
        type = "symbolic link";
    }else if(S_ISSOCK(st.st_mode)){
        type = "socket";
    }else{
        type = "[UNKNOWN MODE]";
    }
    printf("type:%s\n", type);

    return 0;
}

int main(){
    test_stat();
}
