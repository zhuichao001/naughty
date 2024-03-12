all:
	g++ -fPIC -shared -o libfoo.so foo.cc
	g++ -o run_test main.cc -ldl
clean:
	rm -rf libfoo.so  run_test
