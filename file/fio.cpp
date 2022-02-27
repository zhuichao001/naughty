
#include "fio.h"

/**
    //O_CREAT           创建
    //O_EXCL            与O_CREATE并用, 文件存在则返回出错
    //O_WRONLY          只写
    //O_RDONLY          只读
    //O_RDWR            读写
    //O_APPEND          追加写
    //O_CLOEXEC         fork子进程自动关闭该fd

    //S_IRUSR(S_IREAD)  文件拥有者具备读权限
    //S_IWUSR(S_IWRITE) 文件拥有者具备写权限
    //S_IXUSR(S_IEXEC)  文件拥有者具备可执行权限
    //S_IRGRP           用户组具备读权限
    //S_IWGRP           用户组具备写权限
    //S_IXGRP           用户组具备可执行权限
    //S_IROTH           其他用户具备读权限
    //S_IWOTH           其他用户具备写权限
    //S_IXOTH           其他用户具备可执行权限
*/

int fsize(const int fd) {
    struct stat st;
    if (::fstat(fd, &st)==0) {
        return st.st_size;
    }
    printf("failed fstate errno:%d\n", errno);
    return -1;
}

bool exist(const char *path) {
    if (::access(path, F_OK) == 0) {
        return true;
    }
    return false;
}

bool fwriteable(const char *path) {
    if (::access(path, W_OK) == 0) {
        return true;
    }
    return false;
}

bool seekable(const int fd){
    if (::lseek(fd, 0, SEEK_CUR)==-1) {
        return false;
    }
    return true;
}

int open_create(const char *path) {
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
    int fd = ::open(path, O_RDWR | O_CREAT | O_TRUNC, mode);
    if (fd == -1) {
        printf("failed open or create file:%s, errno:%d\n", path, errno);
        return -1;
    }
    return fd;
}

int open_append(const char* path) {
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
    int fd = ::open(path, O_RDWR | O_APPEND, mode);
    if (fd == -1) {
        printf("failed open appending file:%s, errno:%d\n", path, errno);
        return -1;
    }
    return fd;
} 
int open_read(const char* path) {
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
    int fd = ::open(path, O_RDONLY, mode);
    if (fd == -1) {
        printf("failed open reading file:%s, errno:%d\n", path, errno);
        return -1;
    }
    return fd;
}

int create(const char* path) {
    int fd = ::creat(path, 0664);
    if (fd == -1) {
        printf("failed create file:%s, errno:%d\n", path, errno);
        return -1;
    }
    return fd;
}

int read_file(const int fd, std::string &data) {
    if (fd<0) {
        return -1;
    }

    int length = ::fsize(fd);
    if (length<0) {
        return -1;
    }
    data.reserve(length);

    char buf[4096]={0,};
    memset(buf, 0 ,4096);
    ssize_t n =0;
    while ((n = ::read(fd, buf, 4096))!=0) {
        if (n == -1) {
            if (errno == EINTR){
                continue;
            }
            ::close(fd);
            return -1;
        } 
        data.append(buf, n);
    }
    ::close(fd);
    return 0;
}

int read_file(const char* path, std::string &data){
    if (!exist(path)) {
        return -1;
    }
    int fd = ::open(path, O_RDWR, 0664);
    int err = read_file(fd, data);
    ::close(fd);
    return err;
}

int write_file(const int fd, const char *buf, int length) {
    if (fd<0) {
        return -1;
    }
    int n;
    while (length>0 && (n = ::write(fd, buf, 4096))!=0){
        if (n == -1) {
            if (errno == EINTR){
                continue;
            }
            ::close(fd);
            return -1;
        } 
        buf += n;
        length -= n;
    }
    return 0;
}

int write_file(const int fd, const std::string &data) {
    if (fd<0) {
        return -1;
    }

    const char *buf = data.c_str();
    int length = data.size();
    return write_file(fd, buf, length);
}

int write_file(const char* path, const char *buf, const int length){
    if (!exist(path)) {
        return -1;
    }
    int fd = ::open(path, O_RDWR, 0664);
    int err = write_file(fd, buf, length);
    ::close(fd);
    return err;
}

int append_file(const int fd, const std::string &data) {
    if (fd<0) {
        return -1;
    }

    const char *buf = data.c_str();
    int length = data.size();
    int n;
    while (length>0 && (n = ::write(fd, buf, data.size()))!=0) {
        if (n == -1) {
            if (errno == EINTR){
                continue;
            }
            close(fd);
            return -1;
        } 
        buf += n;
        length -=n;
        fprintf(stderr, "n:%d\n",n);
    }
    return 0;
}

int copy_file(const char * src, const char * dst) {
    int rfd = open_read(src);
    int wfd = open_create(dst);
    if (rfd<0 || wfd<0) {
        return -1;
    }

    char buf[4096];
    int n;
    while ((n = ::read(rfd, buf, 4096))!=0) {
        if (n == -1) {
            if (errno == EINTR){
                continue;
            }

            printf("copy reading failed\n");
            ::close(rfd);
            ::close(wfd);
            return -1;
        } 

        ssize_t bytes = ::write(wfd, buf, n);
        if (bytes == -1) {
            printf("copy writing failed\n");
            ::close(rfd);
            ::close(wfd);
            return -1;
        }
    }
    ::close(rfd);
    ::close(wfd);
    return 0;
}

int rename_file(const char * src, const char * dst) {
    int fd = open(dst, O_DIRECTORY|O_RDONLY);
    ::rename(src, dst);
    return ::fsync(fd);
}

int mkdir(const char* path) {
    char tmp[256];
    strcpy(tmp, path);      

    int len = strlen(tmp);             
    if ('/' != tmp[len-1]) {
        strcat(tmp, "/");
        len++;
    }

    for (int i=1; i<len; i++) {
        if ('/' == tmp[i]) {
            tmp[i] = '\0';
            if (::access(tmp, F_OK) != 0) {
                if (::mkdir(tmp, 0777) == -1) {
                    perror("mkdir() failed!");
                    return -1;
                }
            }
            tmp[i] = '/';      
        }
    }
    return 0;
}

int ls(const char *path, std::vector<std::string> &files) {
    struct stat st;
    if (::stat(path, &st) < 0 || !S_ISDIR(st.st_mode)) {
        printf("invalid path: %s\n", path);
        return -1;
    }

    DIR *dir = NULL;
    if(!(dir = opendir(path))) {
        printf("opendir[%s] error: %m\n", path);
        return -1;
    }

    struct dirent *dp = NULL;
    while ((dp = readdir(dir)) != NULL) {
        if ((!strncmp(dp->d_name, ".", 1)) || (!strncmp(dp->d_name, "..", 2))) {
            continue;
        }

        char subpath[256];
        snprintf(subpath, sizeof(subpath) - 1, "%s/%s\0", path, dp->d_name);
        stat(subpath, &st);
        if (!S_ISDIR(st.st_mode)) {
            files.push_back(std::string(subpath));
        } else {
            //printf("dir:%s/\n", dp->d_name);
        }
    }
    closedir(dir);
    return 0;
}
