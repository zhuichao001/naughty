/*
 * 1. create a new temp file (on the same file system!)
 * 2. write data to the temp file
 * 3. fsync() the temp file
 * 4. rename the temp file to the appropriate name
 * 5. fsync() the containing directory
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define USER_ERR 1
#define LIB_ERR  2
#define SYS_ERR  3
#define INTERNAL_ERR 4

const char *content_v1 = "Version 1 of my data.\n";
const char *content_v2 = "Version 2 of my data.\n";

inline ssize_t full_write(int fd, const char *buf, size_t len) {
    ssize_t written = 0;
    size_t to_write = len;
    ssize_t ret;
    int got_zero = 0;

    while (to_write) {
        ret = write(fd, buf, to_write);
        switch (ret) {
            case 0:
                /* shouldn't happen, try again and see if
                 *           * an error is reported */
                if (got_zero)
                    return written;
                got_zero = 1;
                continue;
            case -1:
                if (errno == EINTR)
                    continue;
                return written ? written : -1;

            default:
                written += ret;
                to_write -= ret;
                buf += ret;
                break;
        }
    }
    return written;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", basename(argv[0]));
        exit(USER_ERR);
    }

    // basename and dirname may modify the string passed in
    char *path = strdup(argv[1]);
    if (!path) {
        perror("strdup");
        exit(LIB_ERR);
    }

    // Note that this will truncate the file.
    mode_t old_mode = umask((mode_t)0);
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror("open");
        exit(SYS_ERR);
    }
    umask(old_mode);

    /*
     * You can't write directly to a directory.  fsync, however
     * is allowed on the directory, even when opened read-only.
     */
    char *containing_dir = dirname(path);
    int dir_fd = open(containing_dir, O_RDONLY);
    if (dir_fd < 0) {
        perror("open");
        exit(SYS_ERR);
    }

    size_t message_len = strlen(content_v1);
    int ret = full_write(fd, content_v1, message_len);
    if (ret != (int)message_len) {
        if (ret < 0) {
            perror("write");
            exit(SYS_ERR);
        }
        /*
         * Short write.  This can happen if the file system is full, for example.  
         * In our case, we can't use the * partial data, so just unlink the file.  
         * If the unlink fails, report this to the user.
         */
        if (unlink(argv[1]) < 0)
            perror("unlink");
        exit(SYS_ERR);
    }
    /*
     * Now the data is in the kernel's page cache.  The next step
     * flushes the page cache for this file to disk.
     */
    if (fsync(fd) < 0) {
        perror("fsync");
        exit(SYS_ERR);
    }

    /*
     * Because we just created this file, we also need to ensure that
     * the new directory entry gets flushed to disk.
     */
    if (fsync(dir_fd) < 0) {
        perror("fsync2");
        exit(SYS_ERR);
    }
    if (close(fd) < 0) {
        perror("close");
        exit(SYS_ERR);
    }

    /*
     * Now we have version 1 of our data safely on disk.  Let's start
     * working on version 2 by creating a new file to hold the updates.
     * Note that we are creating the temp file in the same directory
     * as the target file.  The reason for this is to keep the example
     * as simple as possible.
     */

    const char * TMP_FILE_NAME = "__NEWFILE_REPLACING_XXXXXXXXXX";
    int tmp_file_path_len = strlen(containing_dir) + strlen(TMP_FILE_NAME) + 2;
    char *tmp_file_path = (char *)malloc(tmp_file_path_len);
    if (!tmp_file_path) {
        perror("malloc");
        exit(SYS_ERR);
    }
    ret = snprintf(tmp_file_path, tmp_file_path_len, "%s/%s", containing_dir, TMP_FILE_NAME);
    if (ret >= tmp_file_path_len) {
        /*
         * Coding error, there should have been enough room in
         * the tmp_file_path.
         */
        fprintf(stderr, "Internal Error\n");
        exit(INTERNAL_ERR);
    }

    fprintf(stdout, "tmp_file_path:%s\n", tmp_file_path);
    int new_fd = mkstemp(tmp_file_path);
    if (new_fd == -1) {
        perror("mkstemp");
        exit(SYS_ERR);
    }

    message_len = strlen(content_v2);
    ret = full_write(new_fd, content_v2, message_len);
    if (ret != (int)message_len) {
        if (ret < 0) {
            perror("write");
            exit(SYS_ERR);
        }
        /*
         * Short write.  This can happen if the file system is
         * full, for example.  In our case, we can't use the
         * partial data, so just unlink the file.  If unlink
         * fails, notify the user.
         */
        if (unlink(tmp_file_path) < 0)
            perror("unlink");
        exit(SYS_ERR);
    }

    /* ok, now sync the new file out to disk. */
    if (fsync(new_fd) < 0) {
        perror("fsync");
        exit(SYS_ERR);
    }
    if (close(new_fd) < 0) {
        perror("close");
        exit(SYS_ERR);
    }
    /*
     * It wasn't necessary to sync out the directory at this
     * point, since we're not relying on this new file for any
     * user data (at least not this file as it is--we will rely
     * on it after the rename).
     */

    /* now rename the new file to replace the old one */
    if (rename(tmp_file_path, argv[1]) < 0) {
        perror("rename");
        exit(SYS_ERR);
    }
    free(tmp_file_path);

    /* and sync out the directory fd */
    if (fsync(dir_fd) < 0) {
        perror("fsync dir_fd");
        exit(SYS_ERR);
    }
    if (close(dir_fd) < 0) {
        perror("close dir_fd");
        exit(SYS_ERR);
    }
    free(path);

    exit(0);
}
