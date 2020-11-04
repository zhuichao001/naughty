#include <iostream>
#include <thread>


class Functor {
  public:
    void operator()() {
        for (int i = 0; i < 1; i++) {
            std::cout<<"Display Thread"<<std::endl;
        }
    }
};

int main() {
    std::thread thr((Functor()));
    thr.join();
    std::cout<<"Main Thread"<<std::endl;
    return 0;
}
