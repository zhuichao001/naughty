all:
	g++ -g -Wall -std=c++11 -pthread -o test_strerror_t strerror_t.cpp  
	g++ -g -Wall -std=c++11 -o test_map_ops test_map_ops.cpp   -I../..
	g++ -g -o test_avoid_iter_invalid avoid_iter_invalid.cpp
clean:
	rm -f test_strerror_t
	rm -f test_map_ops
	rm -f test_avoid_iter_invalid
