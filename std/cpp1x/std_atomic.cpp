#include <iostream>       // std::cout
#include <atomic>         // std::atomic, std::memory_order_relaxed
#include <thread>         // std::thread

std::atomic<int> on(0);

void set_on(int x) {
    on.store(x, std::memory_order_relaxed);     // set value atomically
}

void get_on() {
    int y=0;
    do {
        y = on.load(std::memory_order_relaxed);  // get value atomically
    } while (y==0);
    std::cout << "on: " << y << '\n';
}

int main () {
    std::thread first(get_on);
    std::thread second(set_on, 9);

    first.join();
    second.join();

    return 0;
}
