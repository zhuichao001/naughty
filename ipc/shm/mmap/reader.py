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
fd = f.fileno()

print(fd)

f2 = open("/dev/shm/shm3", "r+")
fd2 = f2.fileno()

print(fd2)

os.ftruncate(fd, cap)
ptr = mmap.mmap(fd, cap)

time.sleep(100)
print("1:", fd, len(ptr), ptr[0:8])

time.sleep(10)
#os.ftruncate(fd, cap*2)
os.ftruncate(fd, cap*2)
ptr2 = mmap.mmap(fileno=fd, length=cap, offset=cap)
print("2:", fd, len(ptr2), ptr2[0:8])
