all:
	gcc -o test_aio_read aio_read.c -lrt
	gcc -o test_libaio_write libaio_write.c -lrt -laio
clean:
	rm -f test_aio_read
	rm -f test_libaio_write
