
import sys
import mmap

fd  = int(sys.argv[1])

print("worker fd:", fd)
mp = mmap.mmap(fd, 0)
data = mp[:8]
print(data)
#print(data.decode('utf-8'))
