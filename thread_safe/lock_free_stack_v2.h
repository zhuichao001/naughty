//lock-free but not wait-free
//fix：memory leak!!!

#include <atomic>
#include <memory>

template <typename T>
class lock_free_stack {
private:
    struct node_t {
        std::shared_ptr<T> data;
        node_t *next;
        node_t(const T &v):
            data(std::make_shared<T>(v)),
            next(nullptr){
        }
    };

    std::atomic<node_t*> head;

private:
    std::atomic<int> threads_poping;
    std::atomic<node_t*> to_be_deleted;

    void try_reclaim(node_t *node){
        if(threads_poping==1){
            node_t *unused = to_be_deleted.exchange(nullptr);
            if(!--threads_poping){
                delete_nodes(unused);
            }else if(unused){
                append_unused(unused);
            }
            delete node;
        }else{
            node->next = nullptr;
            append_unused(node);
            --threads_poping;
        }
    }

    static void delete_nodes(node_t *node){
        while(node){
            node_t *next = node->next;
            delete node;
            node = next;
        }
    }

    void append_unused(node_t *node){
        node_t *first = node;
        node_t *last = node;
        while(last->next){
            last = last->next;
        }

        last->next = to_be_deleted;
        while(to_be_deleted.compare_exchange_weak(last->next, first)){ }
    }
    
public:
    lock_free_stack():
        head(nullptr),
        threads_poping(0) {
    }

    void push(const T &t){
        node_t *neo = new node_t(t);
        neo->next = head.load();

        while(head.compare_exchange_weak(neo->next, neo)){ } 
    }

    std::shared_ptr<T> pop(){
        --threads_poping;
        node_t *first = head.load();
        while(first && !head.compare_exchange_weak(first, first->next)){ }

        std::shared_ptr<T> res;
        if(first){
            res.swap(first->data);
            first->next = nullptr;
        }

        try_reclaim(first);
        return res;
    }

};
