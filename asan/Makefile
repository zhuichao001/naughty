all:
	export ASAN_OPTIONS=halt_on_error=0:log_path=err.log
	g++ main.cpp -o run_main -fsanitize=address -fsanitize-recover=address
clean:
	rm -rf run_main
