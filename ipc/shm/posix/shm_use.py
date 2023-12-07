import ctypes
import mmap
import os
import stat
import sys


rtld = ctypes.CDLL("librt.so")

def shm_open(name):
    result = rtld.shm_open(
            name,
            ctypes.c_int(os.O_RDWR | os.O_CREAT | os.O_EXCL),
            ctypes.c_ushort(stat.S_IRUSR | stat.S_IWUSR)
            )

    if result == -1:
        raise RuntimeError(os.strerror(ctypes.get_errno()))
    return result

def shm_unlink(name):
    result = rtld.shm_unlink(name)
    if result == -1:
        raise RuntimeError(os.strerror(ctypes.get_errno()))

fid = shm_open("tmp.shm_create")
size = 65536*4096
os.ftruncate(fid, size)
m = mmap.mmap(fid, size)
