#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>
#include "sync-samples.h"

const char *message = "This is very important data!\n";

int main(int argc, char **argv) {
    int ret;
    size_t message_len;
    FILE *fp;
    int fd, dir_fd;
    char *containing_dir;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", basename(argv[0]));
        exit(USER_ERR);
    }

    /*
     * Note that this will truncate the file.
     */
    fp = fopen(argv[1], "w");
    if (!fp) {
        perror("fopen");
        exit(LIB_ERR);
    }

    message_len = strlen(message);
    ret = fwrite(message, message_len, 1, fp);
    if (ret != 1) {
        fprintf(stderr, "fwrite failed: %d", ferror(fp));
        exit(LIB_ERR);
    }
    /*
     * After the fwrite call returns, the data is in libc's stdio
     * buffer (still in the application's address space).  So, the
     * next thing we want to do is flush that buffer.
     */
    if (fflush(fp) != 0) {
        perror("fflush");
        if (errno == EBADF)
            exit(LIB_ERR);
        else
            exit(SYS_ERR);
    }

    /*
     * Now the data is in the kernel's page cache.  The next steps
     * flush the page cache for this file to disk.
     */
    fd = fileno(fp);
    if (fd == -1) {
        perror("fileno");
        exit(LIB_ERR);
    }
    if (fsync(fd) < 0) {
        perror("fsync");
        exit(SYS_ERR);
    }
    /*
     * Because we just created this file, we also need to ensure that
     * the new directory entry gets flushed to disk.
     */
    /*
     * Basename and dirname may modify the string passed in.  We
     * are not reusing argv[1], though, so we won't worry about
     * it.
     */
    containing_dir = dirname(argv[1]);
    /*
     * You can't write directly to a directory.  fsync, however
     * is allowed on the directory, even when opened read-only.
     */
    dir_fd = open(containing_dir, O_RDONLY);
    if (dir_fd < 0) {
        perror("open");
        exit(SYS_ERR);
    }
    if (fsync(dir_fd) < 0) {
        perror("fsync2");
        exit(SYS_ERR);
    }

    /*
     * There really shouldn't be any errors returned from close,
     * here.  However, in the case of memory corruption
     * (overwriting the dir_fd, for example), you can get a failure.
     * Also, the close call can be interrupted, which we don't
     * specifically handle.  The exit will take care of finishing
     * the job.
     */
    if (close(dir_fd) < 0) {
        perror("close");
        exit(SYS_ERR);
    }
    if (fclose(fp) < 0) {
        perror("fclose");
        exit(SYS_ERR);
    }

    exit(0);
}
