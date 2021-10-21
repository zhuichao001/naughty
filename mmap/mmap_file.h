
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

const int MEM_FILE_LIMIT = 20971520; //20M 

class memfile{
    int fd;
    char *mem;
    int memlen;
    int lastoffset;
public:
    memfile():
        fd(-1),
        mem(nullptr),
        memlen(0),
        lastoffset(0){
    }

    ~memfile(){
        if(memlen>0){
            ::munmap(mem, memlen);
        }
    }

    int create(const char *path){
        fd = ::open(path, O_RDWR | O_CREAT , 0664);
        if(fd<0) {
            fprintf(stderr, "open file error: %s\n", strerror(errno));
            ::close(fd);
            return -1;
        }
        ::ftruncate(fd, MEM_FILE_LIMIT);

        mem = (char*)::mmap(nullptr, MEM_FILE_LIMIT, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
        if(mem == MAP_FAILED) {
            fprintf(stderr, "mmap error: %s\n", strerror(errno));
            ::close(fd);
            return -1;
        }

        ::close(fd);
        return 0;
    }

    int load(const char *path){
        fd = ::open(path, O_RDWR, 0664);
        if(fd<0) {
            fprintf(stderr, "open file error: %s\n", strerror(errno));
            ::close(fd);
            return -1;
        }

        struct stat sb;
        if (stat(path, &sb) < 0) {
            fprintf(stderr, "stat %s fail\n", path);
            ::close(fd);
            return -1;
        }

        if(sb.st_size > MEM_FILE_LIMIT) {
            fprintf(stderr, "length:%d is too large\n", sb.st_size);
            ::close(fd);
            return -1;
        }

        mem = (char *)mmap(NULL, MEM_FILE_LIMIT, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (mem == (char *)-1) {
            fprintf(stderr, "mmap fail\n");
            ::close(fd);
            return -1;
        }
        ::close(fd);

        restoremeta();
        return 0;
    }

    void rename(const char *from, const char *to){
        ::rename(from, to); //mv from to
    }

    int size(){
        return memlen;
    }

    int write(int index, const char *data, int len){
        if(data==nullptr||len==0){
            return -1;
        }

        memcpy(mem+memlen, &index, sizeof(int));
        memcpy(mem+memlen+sizeof(int), &len, sizeof(int));
        memcpy(mem+memlen+sizeof(int)*2, data, len);
        msync(mem+memlen, len+sizeof(int)*2, MS_SYNC);

        lastoffset = memlen;
        memlen += sizeof(int)*2+len;
        return 0;
    }

    int read(int index, char **data, int *len){
        int pos =0;
        while(pos<memlen){
            int curidx = *(int*)(mem+pos);
            if(curidx<index){
                pos += sizeof(int)*2 + *(int*)(mem+pos+sizeof(int));
            }else if(curidx==index){
                *data = mem+pos+sizeof(int)*2;
                *len = *(int*)(mem+pos+sizeof(int));
                return 0;
            }else{
                break;
            }
        }
        return -1;
    }

    int firstindex(){
        if(memlen>0){
            return *(int*)mem;
        }
        return 0;
    }

    int lastindex(){
        if(memlen>0 && lastoffset>=0){
            return *(int*)(mem+lastoffset);
        }
        return 0;
    }


    int release(){
        ::close(fd);
        return 0;
    }

private:
    int restoremeta(){
        int pos =0;
        while(pos<MEM_FILE_LIMIT){
            int len = *(int*)(mem+pos+sizeof(int));
            if(len==0){
                break;
            }
            lastoffset = pos;
            pos += sizeof(int)*2 + len;
        }
        memlen = pos;
        return 0;
    }

};
