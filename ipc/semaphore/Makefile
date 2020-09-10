all:
	gcc -g -pthread simple_sem.c -o tsem 
	g++ pshared_sem.c -pthread -o pshared
	g++ named_sem_p.c -pthread -o named_p
	g++ named_sem_c.c -pthread -o named_c
clean:
	rm -f tsem pshared named_p named_c
