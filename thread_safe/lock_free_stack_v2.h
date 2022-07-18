//lock-free but not wait-free
//use reference counter to relaim

#include <atomic>
#include <memory>

template<typename T>
class lock_free_stack {
    struct snode_t;
    struct snode_counter_t {
        int external_count;
        snode_t* ptr;
    };

    struct snode_t {
        std::shared_ptr<T> data;
        std::atomic<int> internal_count;
        snode_counter_t next;
        snode_t(T const &v):
            data(std::make_shared<T>(v)),
            internal_count(0) {
        }
    };

    std::atomic<snode_counter_t> head;
private:
    void increase_head_count(snode_counter_t& old_counter) {
        snode_counter_t new_counter;
        do{
            new_counter = old_counter;
            ++new_counter.external_count;
        }while(!head.compare_exchange_strong(old_counter, new_counter, std::memory_order_acquire, std::memory_order_relaxed));
        old_counter.external_count = new_counter.external_count;
    }
public:
    ~lock_free_stack() {
        while(pop());
    }

    void push(T const& data) {
        snode_counter_t neo;
        neo.ptr = new snode_t(data);
        neo.external_count = 1;
        neo.ptr->next = head.load(std::memory_order_relaxed);
        while(!head.compare_exchange_weak(neo.ptr->next, neo, std::memory_order_release, std::memory_order_relaxed));
    }

    std::shared_ptr<T> pop() {
        snode_counter_t old_head = head.load(std::memory_order_relaxed);
        for(;;) {
            increase_head_count(old_head);
            snode_t* const ptr = old_head.ptr;
            if(!ptr) {
                return std::shared_ptr<T>();
            }
            if(head.compare_exchange_strong(old_head, ptr->next, std::memory_order_relaxed)) {
                std::shared_ptr<T> res;
                res.swap(ptr->data);

                const int count_increase = old_head.external_count-2;
                if(ptr->internal_count.fetch_add(count_increase, std::memory_order_release) == -count_increase) {
                    delete ptr;
                }
                return res;
            } else if(ptr->internal_count.fetch_sub(1, std::memory_order_relaxed)==1) {
                ptr->internal_count.load(std::memory_order_acquire);
                delete ptr;
            }
        }
    }
};
