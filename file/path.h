#include <sys/io.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

bool file_exist(const char *path){
    if(path==nullptr){
        return false;
    }

    if(access(path, F_OK)==0){
        return true;
    }
    return false;
}

bool dir_exist(const char *dir){
    if(dir==nullptr){
        return false;
    }

    if(opendir(dir)==nullptr){
        return false;
    }
    return true;
}

bool make_dir(const char *dir){
    int flag = mkdir(dir, S_IRWXU);
    if(flag == 0){
        return true;
    }
    return false;
}
