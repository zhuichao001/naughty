#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

const int PAGE_SIZE = 4096;

class Memmap{
    int fd_;
    char *mem_;
    int memlen_;
    bool mmaped_=false;
public:
    Memmap():
        fd_(-1),
        mem_(nullptr),
        memlen_(0),
        mmaped_(true){
    }

    ~Memmap(){
        release();
    }

    int create(const char *path, int length, int flags=0){
        assert(length>0);
        length = ((length+PAGE_SIZE-1)/PAGE_SIZE)*PAGE_SIZE;
        fd_ = ::open(path, O_RDWR | O_CREAT , 0664);
        if(fd_<0) {
            fprintf(stderr, "open file error: %s\n", strerror(errno));
            return -1;
        }
        ::ftruncate(fd_, length);

        mem_ = (char*)::mmap(nullptr, length, PROT_READ|PROT_WRITE, MAP_SHARED|flags, fd_, 0);
        if(mem_ == MAP_FAILED) {
            fprintf(stderr, "mmap error: %s\n", strerror(errno));
            ::close(fd_);
            return -1;
        }
        ::close(fd_);

        memlen_ = length;
        mmaped_ = true;
        return 0;
    }

    int load(const char *path, int flags=0){
        fd_ = ::open(path, O_RDWR, 0664);
        if(fd_<0) {
            fprintf(stderr, "open file error: %s\n", strerror(errno));
            return -1;
        }

        struct stat sb;
        if (stat(path, &sb) < 0) {
            fprintf(stderr, "stat %s fail\n", path);
            ::close(fd_);
            return -1;
        }

        const int length = ((sb.st_size+PAGE_SIZE-1)/PAGE_SIZE)*PAGE_SIZE;
        mem_ = (char *)mmap(NULL, length, PROT_READ | PROT_WRITE, flags, fd_, 0);
        if (mem_ == (char *)-1) {
            fprintf(stderr, "mmap fail\n");
            ::close(fd_);
            return -1;
        }
        ::close(fd_);
        fd_=-1;

        memlen_ = length;
        mmaped_ = true;
        return 0;
    }

    char *data(){
        assert(mmaped_);
        return mem_;
    }

    int size(){
        assert(mmaped_);
        return memlen_;
    }

    int write(int index, const char *data, int len, bool is_sync=false, int sync_flags=MS_ASYNC){
        assert(mmaped_);
        if(data==nullptr||len<=0){
            return -1;
        }

        if(index+len>memlen_){
            return -1;
        }

        memcpy(mem_+index, data, len);
        if(is_sync){
            msync(mem_+index, len, sync_flags);
        }

        return 0;
    }

    int read(int index, char *data, int len){
        assert(mmaped_);
        if(data==nullptr||len<=0){
            return -1;
        }

        if(index+len>memlen_){
            return -1;
        }

        memcpy(data, mem_+index, len);
        return 0;
    }

    int release(){
        if(!mmaped_){
            return -1;
        }
        if(fd_>0){ 
            ::close(fd_);
            fd_=-1;
        }
        munmap(mem_, memlen_);
        mem_=nullptr;
        memlen_=-1;
        mmaped_ = false;
        return 0;
    }
};
