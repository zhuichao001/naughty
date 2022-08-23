#include <fcntl.h>

//errno = EACCESS/EAGAIN if failed
#define read_lock(fd, offset, whence, len)\
    lock_reg(fd, F_SETLK, F_RDLCK, offset, whence, len)

//wait until lock success
#define readw_lock(fd, offset, whence, len)\
    lock_reg(fd, F_SETLKW, F_RDLCK, offset, whence, len)

//errno = EACCESS/EAGAIN if failed
#define write_lock(fd, offset, whence, len)\
    lock_reg(fd, F_SETLK, F_WRLCK, offset, whence, len)

//wait until lock success
#define writew_lock(fd, offset, whence, len)\
    lock_reg(fd, F_SETLKW, F_WRLCK, offset, whence, len)

#define un_lock(fd, offset, whence, len)\
    lock_reg(fd, F_SETLK, F_UNLOCK, offset, whence, len)

#define is_read_lockable(fd, offset, whence, len)\
    !lock_test(fd, F_RDLCK, offset, whence, len)

#define is_write_lockable(fd, offset, whence, len)\
    !lock_test(fd, F_WRLCK, offset, whence, len)

int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len){
    struct flock lock;

    lock.l_type = type;  /* F_RDLCK, F_WRLCK, F_UNLCK */
    lock.l_start = offset;
    lock.l_whence = whence; /* SEEK_SET, SEEK_CUR, SEEK_END */
    lock.l_len = len;

    return fcntl(fd, cmd, &lock); /* -1 upon error */
}

pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len){
    struct flock lock;

    lock.l_type = type;
    lock.l_start = offset;
    lock.l_whence = whence;
    lock.l_len = len;

    if(fcntl(fd, F_GETLK, &lock)==-1)
        return -1; /*unexpected error*/

    if(lock.l_type==F_UNLCK)
        return 0; /*false, region not locked by another proc*/

    return lock.l_pid; /*true, return positive PID of lock owner*/
}
