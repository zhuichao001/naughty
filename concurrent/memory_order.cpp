#include <atomic>
#include <thread>
#include <vector>
#include <iostream>

void test1(){ //loose pattern
    std::atomic<int> counter = {0};
    std::vector<std::thread> vt;
    for (int i = 0; i < 64; ++i) {
        vt.emplace_back([&](){
            counter.fetch_add(1, std::memory_order_relaxed);
        });
    }

    for (auto& t : vt) {
        t.join();
    }
    std::cout << "loose pattern, counter:" << counter << std::endl;
}

void test2(){ //produce/consume pattern
    std::atomic<int*> ptr(nullptr);
    int v;

    std::thread producer([&]() {
        int* p = new int(13);
        v = 1024;
        ptr.store(p, std::memory_order_release);
    });

    std::thread consumer([&]() {
        int* p;
        while(!(p = ptr.load(std::memory_order_consume)));
    
        std::cout << "p: " << *p << std::endl;
        std::cout << "v: " << v << std::endl;
    });

    producer.join();
    consumer.join();
}

void test3(){ //acquire/release pattern
    std::vector<int> v;
    std::atomic<int> flag = {0};

    std::thread release([&]() {
        v.push_back(42);
        flag.store(1, std::memory_order_release);
    });

    std::thread acqrel([&]() {
        int expected = 1; // must before compare_exchange_strong
        while(!flag.compare_exchange_strong(expected, 2, std::memory_order_acq_rel)) {
            expected = 1; // must after compare_exchange_strong
        }
        // flag has changed to 2
    });

    std::thread acquire([&]() {
        while(flag.load(std::memory_order_acquire) < 2);

        std::cout << v.at(0) << std::endl; // must be 42
    });

    release.join();
    acqrel.join();
    acquire.join();
}

void test4(){  //linear pattern
    std::atomic<int> counter = {0};
    std::vector<std::thread> vt;
    for (int i = 0; i < 128; ++i) {
        vt.emplace_back([&](){
            counter.fetch_add(1, std::memory_order_seq_cst);
        });
    }

    for (auto& t : vt) {
        t.join();
    }
    std::cout << "linear pattern, counter:" << counter << std::endl;
}


int main(){
    test1();
    test2();
    test3();
    test4();
    return 0;
}
