#include <vector>
#include <string>
#include <chrono>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>


std::string FormatTimeNow(const char* format);

//信号量实现
class Semaphore {
public:
    explicit Semaphore(int count = 0) : count_(count) {
    }

    void Signal() {
        std::unique_lock<std::mutex> lock(mutex_);
        ++count_;
        cv_.notify_one();
    }

    void Wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [=] { return count_ > 0; });
        --count_;
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int count_;
};


Semaphore g_semaphore(2);
std::mutex g_io_mutex;


void Worker() {
    g_semaphore.Wait();

    std::thread::id thread_id = std::this_thread::get_id();

    std::string now = FormatTimeNow("%H:%M:%S");
    {
        std::lock_guard<std::mutex> lock(g_io_mutex);
        std::cout << "Thread " << thread_id << ": wait succeeded" << " (" << now << ")" << std::endl;
    }

    // Sleep 1 second to simulate data processing.
    std::this_thread::sleep_for(std::chrono::seconds(2));

    g_semaphore.Signal();
}

std::string FormatTimeNow(const char* format) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_c);

    char buf[20];
    std::strftime(buf, sizeof(buf), format, now_tm);
    return std::string(buf);
}

int main() {
    const std::size_t SIZE = 6;

    std::vector<std::thread> threads;
    threads.reserve(SIZE);

    for (std::size_t i = 0; i < SIZE; ++i) {
        threads.emplace_back(&Worker);
    }

    for (std::thread& t : threads) {
        t.join();
    }

    return 0;
}
