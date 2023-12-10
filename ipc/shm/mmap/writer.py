import os
import sys
import stat
import mmap
import time
import ctypes

rt = ctypes.CDLL("librt.so")

PAGE_SIZE = 4096

cap = PAGE_SIZE
#fd = rt.shm_open( "shm2", ctypes.c_int(os.O_RDWR | os.O_CREAT), ctypes.c_ushort(stat.S_IRUSR | stat.S_IWUSR))


f = open("/dev/shm/shm2", "r+")
#f = open("shm2", "r+")
fd = f.fileno()

os.ftruncate(fd, cap)
ptr = mmap.mmap(fd, length=cap, offset=0)

ptr[0:8] = b"12345678";

time.sleep(10)

os.ftruncate(fd, PAGE_SIZE*2)
ptr = mmap.mmap(fd, length=PAGE_SIZE, offset=PAGE_SIZE)

ptr[0:8] = b"abcdefgh";

