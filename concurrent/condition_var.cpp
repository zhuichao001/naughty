#include <queue>
#include <chrono>
#include <mutex>
#include <thread>
#include <iostream>
#include <condition_variable>


int main() {
    std::queue<int> pipe;
    std::mutex mtx;
    std::condition_variable cv;

    auto producer = [&]() {
        std::thread::id pid = std::this_thread::get_id();
        for (int i = 0; ; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(900));

            std::unique_lock<std::mutex> lock(mtx);
            std::cout << pid << " producing " << i << std::endl;
            pipe.push(i);
            cv.notify_all(); // notify_one also OK
        }
    };

    auto consumer = [&]() {
        std::thread::id pid = std::this_thread::get_id();
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&pipe](){return !pipe.empty();});
            lock.unlock(); // temporarily unlock for producers continue production

            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            lock.lock();
            while (!pipe.empty()) {
                std::cout << pid << "consuming " << pipe.front() << std::endl;
                pipe.pop();
            }
        }
    };

    std::thread p(producer);
    const int CONSUMERS = 3;
    std::thread cs[CONSUMERS];

    for (int i = 0; i < CONSUMERS; ++i) {
        cs[i] = std::thread(consumer);
    }
    p.join();

    for (int i = 0; i < CONSUMERS; ++i) {
        cs[i].join();
    }
    return 0;
}
