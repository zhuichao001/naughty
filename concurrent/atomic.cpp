#include <atomic>
#include <thread>
#include <iostream>


void test1(){
    std::atomic<int> count = {0};

    std::thread t1([&](){
        count.fetch_add(1);
    });

    std::thread t2([&](){
        count++;        // same as fetch_add
        count += 1;     // same as fetch_add
    });

    t1.join();
    t2.join();
    std::cout << count << std::endl;
}

void test2() {
    struct A {
        float x;
        int y;
        long long z;
    };

    std::atomic<A> a;
    std::cout << std::boolalpha << a.is_lock_free() << std::endl;

    std::atomic<int> b;
    std::cout << std::boolalpha << b.is_lock_free() << std::endl;
}

int main(){
    test1();
    test2();
}
