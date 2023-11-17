import sys
import mmap
import cffi

ffi = cffi.FFI()

ffi.cdef("""
int shm_open(const char *path, int map_size);

""")

lib = ffi.verify(sources=["shm_open.c"]);

path = "tmp.shm_create"
MAP_SIZE = 65536*4096
fd = lib.shm_open(path, MAP_SIZE)
mmap.mmap(int(sys.argv[1]), MAP_SIZE)
