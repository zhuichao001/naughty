all:
	g++ -o test_spinlock_v0 -DUSE_SPINLOCK spin_vs_mutex0.cpp -lpthread
	g++ -o test_mutex_v0  spin_vs_mutex0.cpp -lpthread
	g++ -o test_spinlock_v1 -DUSE_SPINLOCK spin_vs_mutex1.cpp -lpthread
	g++ -o test_mutex_v1  spin_vs_mutex1.cpp -lpthread
	g++ -o test_rwlock test_rwlock.cpp -lpthread
clean:
	rm -rf test_spinlock_v0
	rm -rf test_mutex_v0
	rm -rf test_spinlock_v1
	rm -rf test_mutex_v1
	rm -rf test_rwlock
