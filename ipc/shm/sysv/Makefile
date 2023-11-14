all:
	gcc -g -o shm_rw shm_rw.c
	gcc -g -o share_cnt share_cnt.c -lpthread
	gcc -g -o shm_create shm_create.c -lpthread -lrt
	gcc -g -o shm_addr shm_addr.c -lpthread -lrt
clean:
	rm -f shm_rw
	rm -f shm_create
	rm -f share_cnt
	rm -f shm_addr
