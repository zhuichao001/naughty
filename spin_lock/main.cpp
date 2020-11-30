#include <thread>
#include <mutex>
#include <iostream>
#include "spinlock.h"
 
long long g_count = 0;
spinlock g_lock;
 
void increment() {
    for(long long i=0; i<600000000; ++i) {
	    g_lock.acquire();
	    g_count++;
	    g_lock.release();
    }
    std::cout << std::this_thread::get_id() << ": " << g_count << '\n';
}
 
int main() {
    std::cout << "at the beginning: " << g_count << '\n';
 
    std::thread t1(increment);
    std::thread t2(increment);
 
    t1.join();
    t2.join();
 
    std::cout << "at the end: " << g_count << '\n';
    return 0;
}
