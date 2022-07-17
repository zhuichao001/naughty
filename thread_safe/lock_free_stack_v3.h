//lock-free but not wait-free
//Use hazard pointer to fix memory leak

#include <atomic>
#include <memory>
#include <functional>
#include <stdexcept>
#include <thread>

template<typename N> 
void do_delete(void* p) { 
    delete static_cast<N*>(p); 
}

struct data_to_reclaim { 
    void* data; 
    std::function<void(void*)> deleter; 
    data_to_reclaim* next;

    template<typename N> data_to_reclaim(N* p): // 1 
        data(p), 
        deleter(&do_delete<N>), 
        next(0) {
    }

    ~data_to_reclaim() { 
        deleter(data); // 2 
    } 
};


struct hazard_t { 
    std::atomic<std::thread::id> id; 
    std::atomic<void*> pointer; 
};

unsigned const MAX_HAZARD_POINTERS=100; 
hazard_t hazard_pointers[MAX_HAZARD_POINTERS]; 

class hazard_owner { 
    hazard_t* hp; 
public: 
    hazard_owner(hazard_owner const&)=delete; 
    hazard_owner operator=(hazard_owner const&)=delete; 

    hazard_owner(): hp(nullptr) { 
        for(int i=0; i<MAX_HAZARD_POINTERS; ++i) { 
            std::thread::id id; 
            if(hazard_pointers[i].id.compare_exchange_strong(id, std::this_thread::get_id())) { 
                hp = &hazard_pointers[i]; 
                break;
            } 
        }
        if(!hp){
            throw std::runtime_error("No hazard pointers available"); 
        } 
    }

    std::atomic<void*>& get_pointer() { 
        return hp->pointer; 
    }

    ~hazard_owner() {
        hp->pointer.store(nullptr);
        hp->id.store(std::thread::id());
    }
};

std::atomic<void*>& get_hazard_pointer_for_current_thread() {
    thread_local static hazard_owner hazard;
    return hazard.get_pointer();
}

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
    std::atomic<data_to_reclaim*> nodes_to_reclaim; 

    bool outstanding_hazard_pointers_for(void* p) {
        for(unsigned i=0; i<MAX_HAZARD_POINTERS; ++i) {
            if(hazard_pointers[i].pointer.load()==p) {
                return true;
            }
        }
        return false;
    }

    void add_to_reclaim_list(data_to_reclaim* node){
        node->next=nodes_to_reclaim.load(); 
        while(!nodes_to_reclaim.compare_exchange_weak(node->next, node)); 
    }
    
    template<typename N> 
    void reclaim_later(N* data){
        add_to_reclaim_list(new data_to_reclaim(data));
    }
    
    void delete_nodes_with_no_hazards(){
        data_to_reclaim* current=nodes_to_reclaim.exchange(nullptr);
        while(current){
            data_to_reclaim* const next=current->next;
            if(!outstanding_hazard_pointers_for(current->data)){
                delete current;
            }else{
                add_to_reclaim_list(current);
            }
            current=next;
        }
    }

public:
    lock_free_stack():
        head(nullptr) {
    }

    void push(const T &t){
        node_t *neo = new node_t(t);
        neo->next = head.load();

        while(head.compare_exchange_weak(neo->next, neo)){ } 
    }

    std::shared_ptr<T> pop(){
        std::atomic<void*>& hp = get_hazard_pointer_for_current_thread(); 
        node_t *old_head = head.load(); 
        do{ 
            node_t *temp; 
            do{
                temp = old_head; 
                hp.store(old_head); 
                old_head=head.load(); 
            } while(old_head!=temp); 
        }while(old_head && !head.compare_exchange_strong(old_head, old_head->next)); 
        hp.store(nullptr);

        std::shared_ptr<T> res;
        if(old_head){
            res.swap(old_head->data);
            if(outstanding_hazard_pointers_for(old_head)){
                reclaim_later(old_head);
            }else{ 
                delete old_head;
            }
            delete_nodes_with_no_hazards();
        }return res;
    }
};
