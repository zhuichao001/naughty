#include <stdio.h>
#include <sys/statvfs.h>

int main(int argc, char* argv[]){
    struct statvfs vfs;
    statvfs(argv[1], &vfs);
    printf("f_bsize (block size): %lu\n"
        "f_frsize (fragment size): %lu\n"
        "f_blocks (size of fs in f_frsize units): %lu\n"
        "f_bfree (free blocks): %lu\n"
        "f_bavail free blocks for unprivileged users): %lu\n"
        "f_files (inodes): %lu\n"
        "f_ffree (free inodes): %lu\n"
        "f_favail (free inodes for unprivileged users): %lu\n"
        "f_fsid (file system ID): %lu\n"
        "f_flag (mount flags): %lu\n"
        "f_namemax (maximum filename length)%lu\n",
        vfs.f_bsize,
        vfs.f_frsize,
        vfs.f_blocks,
        vfs.f_bfree,
        vfs.f_bavail,
        vfs.f_files,
        vfs.f_ffree,
        vfs.f_favail,
        vfs.f_fsid,
        vfs.f_flag,
        vfs.f_namemax);

    return 0;
}
