#include <thread>
#include <mutex>
#include <iostream>
#include <string>

thread_local int cnt = 1;
std::mutex mux;

void increase(const std::string& thread_name) {
    ++cnt; // modifying thread-local variable outside lock is okay
    std::lock_guard<std::mutex> lock(mux);
    fprintf(stdout, "Counter for thread-%s: %d\n", thread_name.c_str(), cnt);
}

int main() {
    std::thread ta(increase, "a");
    std::thread tb(increase, "b");

    {
        std::lock_guard<std::mutex> lock(mux);
        fprintf(stdout, "Counter for thread-main: %d\n", cnt);
    }

    ta.join();
    tb.join();
    return 0;
}
