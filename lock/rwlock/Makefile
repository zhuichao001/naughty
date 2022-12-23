all:
	g++ -o test_posix_rwlock test_posix_rwlock.cpp -lpthread
	g++ -o test_thread_rwlock test_thread_rwlock.cpp -lpthread
	g++ -o test_2pl_lockfree test_2pl_lockfree.cpp -lpthread
clean:
	rm -rf test_posix_rwlock 
	rm -rf test_thread_rwlock
	rm -rf test_2pl_lockfree
