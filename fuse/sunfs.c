#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/xattr.h>

static int sun_getattr(const char *path, struct stat *stbuf) {
    int res = lstat(path, stbuf);
    if (res == -1)
        return -errno;

    return 0;
}

static int sun_access(const char *path, int mask) {
    int res = access(path, mask);
    if (res == -1)
        return -errno;

    return 0;
}

static int sun_readlink(const char *path, char *buf, size_t size) {
    int res = readlink(path, buf, size - 1);
    if (res == -1)
        return -errno;

    buf[res] = '\0';
    return 0;
}


static int sun_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    DIR *dp;
    struct dirent *de;

    (void) offset;
    (void) fi;

    dp = opendir(path);
    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        if (filler(buf, de->d_name, &st, 0))
            break;
    }

    closedir(dp);
    return 0;
}

static int sun_mknod(const char *path, mode_t mode, dev_t rdev) {
    int res;

    /* On Linux this could just be 'mknod(path, mode, rdev)' but this is more portable */
    if (S_ISREG(mode)) {
        res = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
        if (res >= 0)
            res = close(res);
    } else if (S_ISFIFO(mode))
        res = mkfifo(path, mode);
    else
        res = mknod(path, mode, rdev);
    if (res == -1)
        return -errno;

    return 0;
}

static int sun_mkdir(const char *path, mode_t mode) {
    int res = mkdir(path, mode);
    if (res == -1)
        return -errno;

    return 0;
}

static int sun_unlink(const char *path) {
    int res = unlink(path);
    if (res == -1)
        return -errno;

    return 0;
}

static int sun_rmdir(const char *path) {
    int res = rmdir(path);
    if (res == -1)
        return -errno;

    return 0;
}

static int sun_symlink(const char *from, const char *to) {
    int res = symlink(from, to);
    if (res == -1)
        return -errno;

    return 0;
}

static int sun_rename(const char *from, const char *to) {
    int res = rename(from, to);
    if (res == -1)
        return -errno;

    return 0;
}

static int sun_link(const char *from, const char *to) {
    int res = link(from, to);
    if (res == -1)
        return -errno;

    return 0;
}

static int sun_chmod(const char *path, mode_t mode) {
    int res = chmod(path, mode);
    if (res == -1)
        return -errno;

    return 0;
}

static int sun_chown(const char *path, uid_t uid, gid_t gid) {
    int res = lchown(path, uid, gid);
    if (res == -1)
        return -errno;

    return 0;
}

static int sun_truncate(const char *path, off_t size) {
    int res = truncate(path, size);
    if (res == -1)
        return -errno;

    return 0;
}

static int sun_open(const char *path, struct fuse_file_info *fi) {
    int res = open(path, fi->flags);
    if (res == -1)
        return -errno;

    close(res);
    return 0;
}

static int sun_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    int fd;

    (void) fi;
    fd = open(path, O_RDONLY);
    if (fd == -1)
        return -errno;

    int res = pread(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    close(fd);
    return res;
}

static int sun_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {

    (void) fi;
    int fd = open(path, O_WRONLY);
    if (fd == -1)
        return -errno;

    int res = pwrite(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    close(fd);
    return res;
}

static int sun_statfs(const char *path, struct statvfs *stbuf) {
    int res = statvfs(path, stbuf);
    if (res == -1)
        return -errno;

    return 0;
}

static int sun_release(const char *path, struct fuse_file_info *fi) {
    /* Just a stub.  This method is optional and can safely be left unimplemented */
    (void) path;
    (void) fi;
    return 0;
}

static int sun_fsync(const char *path, int isdatasync, struct fuse_file_info *fi) {
    /* Just a stub.  This method is optional and can safely be left unimplemented */
    (void) path;
    (void) isdatasync;
    (void) fi;
    return 0;
}

static int sun_fallocate(const char *path, int mode, off_t offset, off_t length, struct fuse_file_info *fi) {
    int fd;

    (void) fi;

    if (mode)
        return -EOPNOTSUPP;

    fd = open(path, O_WRONLY);
    if (fd == -1)
        return -errno;

    int res = -posix_fallocate(fd, offset, length);

    close(fd);
    return res;
}

/* xattr operations are optional and can safely be left unimplemented */
static int sun_setxattr(const char *path, const char *name, const char *value, size_t size, int flags) {
    int res = lsetxattr(path, name, value, size, flags);
    if (res == -1)
        return -errno;
    return 0;
}

static int sun_getxattr(const char *path, const char *name, char *value, size_t size) {
    int res = lgetxattr(path, name, value, size);
    if (res == -1)
        return -errno;
    return res;
}

static int sun_listxattr(const char *path, char *list, size_t size) {
    int res = llistxattr(path, list, size);
    if (res == -1)
        return -errno;
    return res;
}

static int sun_removexattr(const char *path, const char *name) {
    int res = lremovexattr(path, name);
    if (res == -1)
        return -errno;
    return 0;
}

static struct fuse_operations sun_operations = {
    .getattr    = sun_getattr,
    .access     = sun_access,
    .readlink   = sun_readlink,
    .readdir    = sun_readdir,
    .mknod      = sun_mknod,
    .mkdir      = sun_mkdir,
    .symlink    = sun_symlink,
    .unlink     = sun_unlink,
    .rmdir      = sun_rmdir,
    .rename     = sun_rename,
    .link       = sun_link,
    .chmod      = sun_chmod,
    .chown      = sun_chown,
    .truncate   = sun_truncate,
    .open       = sun_open,
    .read       = sun_read,
    .write      = sun_write,
    .statfs     = sun_statfs,
    .release    = sun_release,
    .fsync      = sun_fsync,
    .fallocate  = sun_fallocate,
    .setxattr   = sun_setxattr,
    .getxattr   = sun_getxattr,
    .listxattr  = sun_listxattr,
    .removexattr= sun_removexattr,
};

int main(int argc, char *argv[]) {
    umask(0);
    return fuse_main(argc, argv, &sun_operations, NULL);
}
