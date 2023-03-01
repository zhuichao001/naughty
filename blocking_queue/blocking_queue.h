#include <iostream>
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
        //std::cout << "to pop.1" << std::endl;
        std::unique_lock<std::mutex> lock(mutex_);
        //std::cout << "to pop.2" << std::endl;
        convar_have_.wait(lock, [=]{return !queue_.empty();});
        //std::cout << "to pop.3" << std::endl;
        T t(std::move(queue_.back()));
        //std::cout << "to pop.4" << std::endl;
        queue_.pop_back();
        //std::cout << "to pop.5" << std::endl;
        convar_rest_.notify_one();
        //std::cout << "to pop.6" << std::endl;
        return t;
    }
};
