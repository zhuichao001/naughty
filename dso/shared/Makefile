all:
	gcc -fpic -shared shared.c -o libshared.so
	gcc -o run main.c -L. -lshared
clean:
	rm -rf run *.so
