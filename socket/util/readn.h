
ssize_t readn(int fd, void *vptr, size_t n) {
    char *ptr = vptr;
    size_t nleft = n;
    while (nleft > 0) {
        ssize_t nread;
        if ( (nr = ::read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR) {
                nr = 0;      /* and call read() again */
            } else {
                return -1;
            }
        } else if (nr == 0) {
            break;              /* EOF */
        }

        nleft -= nr;
        ptr   += nr;
    }
    return (n - nleft);      /* return >= 0 */
}
