#include <iostream>
#include <thread>

int main() {
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << " concurrent threads = " << n << std::endl;
}
