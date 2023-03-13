#include <deque>
#include <mutex>
#include <condition_variable>

//Thread-safe blocking queue

template <typename T>
class BlockingQueue {
private:
    const int capacity_;
    std::deque<T>   queue_;
    std::mutex      mutex_;
    std::condition_variable cv_notfull_;
    std::condition_variable cv_notempty_;

public:
    BlockingQueue(int n):
        capacity_(n){
    }

    void push(T const& t) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_notfull_.wait(lock, [&]{return queue_.size()!=capacity_;});
            queue_.push_front(t);
        }
        cv_notempty_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_notempty_.wait(lock, [&]{return !queue_.empty();});
        T t(std::move(queue_.back()));
        queue_.pop_back();
        cv_notfull_.notify_one();
        return t;
    }
};
