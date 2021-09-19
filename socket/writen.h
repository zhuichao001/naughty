
ssize_t writen(int fd, const void *vptr, size_t n) {
    const char  *ptr = vptr;
    size_t nleft = n;
    while (nleft > 0) {
        ssize_t nw;
        if ( (nw = ::write(fd, ptr, nleft)) <= 0) {
            if (nw < 0 && errno == EINTR) {
                nw = 0;       /* and call write() again */
            } else {
                return -1;         /* error */
            }
        }

        nleft -= nw;
        ptr   += nw;
    }
    return n;
}
