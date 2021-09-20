all:
	g++ -o run_cv condition_var.cpp -lpthread
	g++ -o run_atomic atomic.cpp -latomic -lpthread
	g++ -o run_memorder memory_order.cpp -latomic -lpthread
clean:
	rm -rf run_cv
	rm -rf run_atomic
	rm -rf run_memorder
