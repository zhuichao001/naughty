all:
	gcc -g -pthread simple_sem.c -o tsem 
	g++ pshared_sem.c -pthread -o pshared
	g++ named_sem_w.c -pthread -o named_w
	g++ named_sem_r.c -pthread -o named_r
	g++ sequential_sem.c -pthread -o sequential_run
clean:
	rm -f tsem pshared named_r named_w sequential_run
