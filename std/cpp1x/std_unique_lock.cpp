#include <iostream>
#include <thread>
#include <mutex>

int g_val = 1;

void critical_section(int d) {
    static std::mutex mtx;
    std::unique_lock<std::mutex> lock(mtx);

    std::cout << "g_val:" << g_val << std::endl;
    lock.unlock();

    //here can do other things

    lock.lock();
    g_val += d;
}

int main() {
    std::thread t1(critical_section, 2); 
    std::thread t2(critical_section, 3);
    t1.join();
    t2.join();

    std::cout << g_val << std::endl;
    return 0;
}
