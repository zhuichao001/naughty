all:
	g++ -o test_fread fread.cpp
	g++ -o test_fwrite fwrite.cpp -std=c++11 -lpthread
	g++ -o test_pread pread.cpp -I..
	g++ -o test_writev writev.cpp -I..
	g++ -o test_mmap mmap.cpp -I..
	g++ -o test_multhread_write mthread_write.cpp -std=c++11 -lpthread
	g++ -o test_mulproc_write mprocess_write.cpp -std=c++11 -lpthread
clean:
	rm -f test_fread
	rm -f test_fwrite
	rm -f test_pread
	rm -f test_writev
	rm -f test_mmap
	rm -f test_multhread_write
	rm -f test_mulproc_write
	rm -f *.txt
