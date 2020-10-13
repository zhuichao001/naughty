#include <thread>
#include <mutex>
#include <iostream>
#include "spin_lock.h"
 
int g_count = 0;
spin_lock g_lock;
 
void exclusive_increment() {
    for(int i=0;i<1000000;++i) {
	g_lock.acquire();
	g_count++;
	g_lock.release();
    }

    std::cout << std::this_thread::get_id() << ": " << g_count << '\n';
}
 
int main() {
    std::cout << "at the beginning: " << g_count << '\n';
 
    std::thread t1(exclusive_increment);
    std::thread t2(exclusive_increment);
 
    t1.join();
    t2.join();
 
    std::cout << "at the end: " << g_count << '\n';
    return 0;
}
