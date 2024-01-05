#ifndef _UTIL_FIO_H_
#define _UTIL_FIO_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory.h>
#include <string>
#include <vector>

namespace fio{

enum OPEN_PURPOSE{
    CREATE = 1,
    APPEND = 2,
    READ = 3,
};

int fopen(const char *path, OPEN_PURPOSE purpose=READ);

int fcreate(const char* path);

int fsize(const int fd);

bool fwriteable(const char *path);

bool fseekable(const int fd);

int fread(const int fd, std::string &data);

int fread(const char* path, std::string &data);

int fwrite(const int fd, const char *buf, int length);

int fwrite(const int fd, const std::string &data);

int fwrite(const char* path, const char *buf, const int length);

int fappend(const int fd, const std::string &data);

int fcopy(const char * src, const char * dst);

int frename(const char * src, const char * dst);

bool fexist(const char *path);

inline int flseek(const int fd, const int offset) { return ::lseek(fd, offset, SEEK_SET); }

inline int fclose(int fd) { return ::close(fd); }

bool dexist(const char *dir);

int mkdir(const char* path);

int flist(const char *path, std::vector<std::string> &files);

int dircap(const char* path);

} // end of namespace fio

#endif
