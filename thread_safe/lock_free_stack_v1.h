//lock-free but not wait-free
//use shared_ptr to relaim

#include <atomic>
#include <memory>

template <typename T>
class lock_free_stack {
private:
    struct snode_t {
        std::shared_ptr<T> data;
        std::shared_ptr<snode_t> next;
        snode_t(const T &v):
            data(std::make_shared<T>(v)),
            next(nullptr){
        }
    };

    std::shared_ptr<snode_t> head;
public:
    lock_free_stack():
        head(nullptr){
        //assert(std::atomic_is_lock_free(&head));
    }

    void push(const T &v){
        std::shared_ptr<snode_t> const neo = std::make_shared<snode_t>(v);
        neo->next = std::atomic_load(&head);
        while(!std::atomic_compare_exchange_weak(&head, &neo->next, neo));
    }

    std::shared_ptr<T> pop(){
        std::shared_ptr<snode_t> first = std::atomic_load(&head);
        while(first && !std::atomic_compare_exchange_weak(&head, &first, first->next));
        return first ? first->data : std::shared_ptr<T>();
    }
};
