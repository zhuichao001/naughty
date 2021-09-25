#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>


void func0() {
    for (int i = 0; i < 1; i++) {
        std::cout<<"Display Thread"<<std::endl;
    }
}

void func1(int n){
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread " << n << " executing\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void func2(int& n){
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread 2 executing\n";
        ++n;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void test0(){
    std::thread t(func0);
    t.join();
}

void test1(){
    int n = 0;
    std::thread t1; // t1 is not a thread
    std::thread t2(func1, n + 1); // pass by value
    t2.join();
    std::cout << "Final value of n is " << n << '\n';
}

void test2(){
    int n = 0;
    std::thread t3(func2, std::ref(n)); // pass by reference
    std::thread t4(std::move(t3)); // t4 is now running f2(). t3 is no longer a thread
    t4.join();
    std::cout << "Final value of n is " << n << '\n';
}


int main() {
    test0();
    test1();
    test2();
    return 0;
}
