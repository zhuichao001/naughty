#include <deque>
#include <mutex>
#include <condition_variable>

//Thread safty blocking queue

template <typename T>
class BlockingQueue {
private:
    const int capacity_;
    std::deque<T>   queue_;
    std::mutex      mutex_;
    std::condition_variable convar_rest_;
    std::condition_variable convar_have_;

public:
    BlockingQueue(int n):
        capacity_(n){
    }

    void push(T const& t) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            convar_rest_.wait(lock, [=]{return queue_.size()!=capacity_;});
            queue_.push_front(t);
        }
        convar_have_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        convar_have_.wait(lock, [=]{return !queue_.empty();});
        T t(std::move(queue_.back()));
        queue_.pop_back();
        convar_rest_.notify_one();
        return t;
    }
};
