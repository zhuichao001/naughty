#include <thread>
#include <mutex>
#include <iostream>
 
int g_count = 0;
std::mutex g_mutex;
 
void exclusive_increment() {
    const std::lock_guard<std::mutex> lock(g_mutex);

    for(int i=0;i<1000000;++i) g_count++;

    std::cout << std::this_thread::get_id() << ": " << g_count << '\n';
    // g_i_mutex is automatically released when lock goes out of scope
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
