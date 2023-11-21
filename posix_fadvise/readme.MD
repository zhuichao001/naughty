## posix_fadvise() appeared in kernel 2.5.60.
```
#define _XOPEN_SOURCE 600 
#include <fcntl.h> 
int posix_fadvise(int fd, off_t offset, off_t len, int advice);
```

  - Under Linux, POSIX_FADV_NORMAL sets the readahead window to the default size for the backing device; POSIX_FADV_SEQUENTIAL doubles this size, andPOSIX_FADV_RANDOM disables file readahead entirely. These changes affect the entire file, not just the specified region (but other open file handles to the same file are unaffected).

  - POSIX_FADV_WILLNEED and POSIX_FADV_NOREUSE both initiate a non-blocking read of the specified region into the page cache. The amount of data read may be decreased by the kernel depending on VM load. (A few megabytes will usually be fully satisfied, and more is rarely useful.)

  - POSIX_FADV_DONTNEED attempts to free cached pages associated with the specified region. This is useful, for example, while streaming large files. A program may periodically request the kernel to free cached data that has already been used, so that more useful cached pages are not discarded instead.

  - Pages that have not yet been written out will be unaffected, so if the application wishes to guarantee that pages will be released, it should call fsync() or fdatasync() first.


