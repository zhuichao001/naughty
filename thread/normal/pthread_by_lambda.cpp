#include <iostream>
#include <thread>


int main() {
    std::thread thr([]{
        for (int i = 0; i < 1; i++) {
            std::cout<<"Display Thread"<<std::endl;
        }
    });

    thr.join();
    std::cout<<"Main Thread"<<std::endl;
    return 0;
}
