import ctypes
import mmap
import os
import stat
import sys
import numpy as np


try:
    unicode
except NameError:
    unicode = str


rtld = ctypes.cdll.LoadLibrary(None)

def shm_open(name):
    if isinstance(name, bytes):
        name = ctypes.create_string_buffer(name)
    elif isinstance(name, unicode):
        name = ctypes.create_unicode_buffer(name)
    else:
        raise TypeError("`name` must be `bytes` or `unicode`")

    result = rtld.shm_open(
            name,
            ctypes.c_int(os.O_RDWR | os.O_CREAT | os.O_EXCL),
            ctypes.c_ushort(stat.S_IRUSR | stat.S_IWUSR)
            )

    if result == -1:
        raise RuntimeError(os.strerror(ctypes.get_errno()))

    return result


def shm_unlink(name):
    if isinstance(name, bytes):
        name = ctypes.create_string_buffer(name)
    elif isinstance(name, unicode):
        name = ctypes.create_unicode_buffer(name)
    else:
        raise TypeError("`name` must be `bytes` or `unicode`")

    result = rtld.shm_unlink(name)
    if result == -1:
        raise RuntimeError(os.strerror(ctypes.get_errno()))



fid = shm_open("tmp.shm_create")
size = 65536*4096
os.ftruncate(fid, size)
m = mmap.mmap(fid, size)
a = np.frombuffer(m, dtype=np.uint8)
