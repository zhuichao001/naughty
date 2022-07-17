//lock-free but not wait-free
//memory leak!!!

#include <atomic>
#include <memory>

template <typename T>
class lock_free_stack {
private:
    struct node {
        std::shared_ptr<T> data;
        node *next;
        node(const T &v):
            data(std::make_shared<T>(v)),
            next(nullptr){
        }
    };

    std::atomic<node*> head;
public:
    lock_free_stack():
        head(nullptr){
    }

    void push(const T &t){
        node *neo = new node(t);
        neo->next = head.load();

        while(head.compare_exchange_weak(neo->next, neo)){ } 
    }

    std::shared_ptr<T> pop(){
        node *first = head.load();
        while(first && !head.compare_exchange_weak(first, first->next)){ }

        return first ? first->data : std::shared_ptr<T>();
    }

};
