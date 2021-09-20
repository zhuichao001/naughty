#include <iostream>
#include <thread>
#include <mutex>

std::mutex cout_mutex;

void thread_func(const std::string& thread_name) {
    thread_local static int x = 1;
    for (int i = 0; i < 3; ++i) {
        x++;
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "thread[" << thread_name << "]: x = " << x << std::endl;
    }
    return;
}

int main() {
    std::thread t1(thread_func, "t1");
    std::thread t2(thread_func, "t2");

    t1.join();
    t2.join();

    return 0;
}
