#include <iostream>
#include <mutex>  // For std::unique_lock
#include <shared_mutex>
#include <thread>

class ThreadSafeCounter {
public:
    ThreadSafeCounter() = default;

    // Multiple threads/readers can read the counter's value at the same time.
    unsigned int get() const {
        std::shared_lock lock(mutex_);
        return value_;
    }

    // Only one thread/writer can increment/write the counter's value.
    void increment() {
        std::unique_lock lock(mutex_);
        value_++;
    }

    // Only one thread/writer can reset/write the counter's value.
    void reset() {
        std::unique_lock lock(mutex_);
        value_ = 0;
    }

private:
    mutable std::shared_mutex mutex_;
    unsigned int value_ = 0;
};

int main() {
    ThreadSafeCounter counter;

    auto increment_and_print = [&counter]() {
        for (int i = 0; i < 3; i++) {
            counter.increment();
            std::cout << std::this_thread::get_id() << ' ' << counter.get() << '\n';

            // Note: Writing to std::cout actually needs to be synchronized as well
            // by another std::mutex. This has been omitted to keep the example small.
        }
    };

    std::thread thread1(increment_and_print);
    std::thread thread2(increment_and_print);

    thread1.join();
    thread2.join();
}
