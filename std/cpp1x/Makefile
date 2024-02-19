all:
	g++ -g -Wall -std=c++11 -o test_move std_move.cpp  
	g++ -g -Wall -std=c++11 -o test_bind std_bind.cpp  
	g++ -g -Wall -std=c++11 -o test_function std_function.cpp  
	g++ -g -Wall -std=c++11 -o test_emplace_back std_emplace_back.cpp  
	g++ -g -Wall -std=c++11 -o test_shared_from_this std_shared_from_this.cpp  
	g++ -g -Wall -std=c++11 -o test_cpp11 test_cpp11.cpp
	g++ -g -Wall -std=c++11 -pthread -o test_lock_guard std_lock_guard.cpp  
	g++ -g -Wall -std=c++11 -pthread -o test_thread_local thread_local.cpp  
	g++ -g -Wall -std=c++11 -pthread -o test_atomic std_atomic.cpp  
	g++ -g -Wall -std=c++11 -pthread -o test_mutex std_mutex.cpp  
	g++ -g -Wall -std=c++17 -pthread -o test_shared_mutex std_shared_mutex.cpp  
	g++ -g -Wall -std=c++11 -pthread -o test_unique_lock std_unique_lock.cpp  
	g++ -g -Wall -std=c++11 -pthread -o test_array std_array.cpp  
	g++ -g -Wall -std=c++17 -o test_par_sort std_par_sort.cpp  
	g++ -g -Wall -std=c++17 -o test_fold_expr fold_expr.cpp  
	g++ -g -Wall -std=c++17 -o test_std_scoped_lock std_scoped_lock.cpp -lpthread
clean:
	rm -f test_move
	rm -f test_bind
	rm -f test_function
	rm -f test_emplace_back
	rm -f test_shared_from_this
	rm -f test_cpp11
	rm -f test_lock_guard
	rm -f test_thread_local
	rm -f test_atomic
	rm -f test_mutex
	rm -f test_shared_mutex
	rm -f test_unique_lock
	rm -f test_array
	rm -f test_par_sort
	rm -f test_fold_expr
	rm -f test_std_scoped_lock
