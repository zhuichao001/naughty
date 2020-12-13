#include <iostream>
#include <thread>


void func() {
    for (int i = 0; i < 1; i++) {
        std::cout<<"Display Thread"<<std::endl;
    }
}

int main() {
    std::thread thr(func);
    thr.join();
    std::cout<<"Main Thread"<<std::endl;
    return 0;
}
