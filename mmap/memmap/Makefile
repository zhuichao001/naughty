all:
	g++ -o run_test_mmap_lock test_mmap_lock.cpp 
	dd if=/dev/zero of=20m.dat bs=1M count=20
clean:
	rm -rf run_test_mmap_lock
	rm -rf *.dat
