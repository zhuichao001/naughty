all:
	g++ -o test_if_extend if_extend_threads.cpp -std=c++11 -lpthread
	g++ -o test_vfork vfork.cpp
	g++ -o test_cmd command.cpp
	g++ -o test_clone_vm clone_vm.cpp
clean:
	rm -f test_if_extend
	rm -f test_vfork
	rm -f test_cmd
	rm -f test_clone_vm
