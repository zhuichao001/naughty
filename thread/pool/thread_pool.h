#ifndef __NAUGHTY_THREAD_POOL_H__
#define  __NAUGHTY_THREAD_POOL_H__

#include <vector>
#include <queue>
#include <atomic>
#include <future>
#include <condition_variable>
#include <thread>
#include <functional>
#include <stdexcept>
#include <assert.h>
#include <stdio.h>

class thread_pool_t{
public:
    typedef std::function<void()> task_t;

    thread_pool_t(size_t n):size(0),idle(0){
        scaleup(n);
    }

    ~thread_pool_t(){
        running = false;
        cv.notify_all(); 
        for(std::thread &th : threads){
            if(th.joinable()){
                th.join();
            }
        }
    }

    template<class F, class... A>
    decltype(auto) post(F&& func, A&&... args){
        if(!running){
            throw runtime_error("post failed: thread_pool is not running.");
        }
        using return_type = typename std::result_of_t<F(Args...)>;
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(forward<F>(func), std::forward<A>(args)...));
        future<return_type> res = task->get_future();
        if(true){
            std::lock_guard<std::mutex> locker{mtx};
            tasks.emplace( [task](){(*task)();} );
        }
        if(idle<1){
            scaleup(1);
        }
        cv.notify_one(); 
        return res;
    }

private:
    thread_pool_t(const thread_pool_t &) = delete;
    thread_pool_t(thread_pool_t &&) = delete;
    thread_pool_t &operator=(const thread_pool_t &) = delete;
    thread_pool_t &operator=(thread_pool_t &&) = delete;

    void scaleup(int n){
        for(int i=0; i<n; ++i){
            threads.emplace_back([&,this]{
                while(this->running){
                    task_t task;
                    if(true){
                        std::unique_lock<std::mutex> locker{mtx};    
                        cv.wait(locker, [this]{return !this->running||!this-tasks.empty();});
                        if(!this->running && this->tasks.empty()){
                            return;
                        }
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    --idle;
                    task();
                    ++idle;
                }
            });
            ++size;
        }
    }

    std::vector<std::thread> threads;
    std::queue<task_t> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> running;
    std::atomic<int> idle;
    std::atomic<int> size;
};

#endif
