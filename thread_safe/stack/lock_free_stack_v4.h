//lock-free but not wait-free
//Use hazard-pointer to release memory 

#include <atomic>
#include <memory>
#include <functional>
#include <stdexcept>
#include <thread>

template<typename N> 
void default_delete(void* p) { 
    delete static_cast<N*>(p); 
}

struct reclaim_t { 
    void *data; 
    std::function<void(void*)> deleter; 
    reclaim_t *next;

    template<typename N> reclaim_t(N* p):
        data(p), 
        deleter(&default_delete<N>), 
        next(nullptr) {
    }

    ~reclaim_t() { 
        deleter(data);
    } 
};

struct hazard_t { 
    std::atomic<std::thread::id> id; 
    std::atomic<void*> pointer; 
};

unsigned const MAX_HAZARD_POINTERS=128; 
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

/******************Above is hazard pointer****************/

template <typename T>
class lock_free_stack {
private:
    struct snode_t {
        std::shared_ptr<T> data;
        snode_t *next;
        snode_t(const T &v):
            data(std::make_shared<T>(v)),
            next(nullptr){
        }
    };

    std::atomic<snode_t*> head;

private:
    std::atomic<reclaim_t*> reclaim_head; 

    bool unreleased_hazard_pointer(void* p) {
        for(unsigned i=0; i<MAX_HAZARD_POINTERS; ++i) {
            if(hazard_pointers[i].pointer.load()==p) {
                return true;
            }
        }
        return false;
    }

    void add_to_reclaim_list(reclaim_t* node){
        node->next = reclaim_head.load(); 
        while(!reclaim_head.compare_exchange_weak(node->next, node)); 
    }
    
    template<typename N> 
    void reclaim_later(N* data){
        add_to_reclaim_list(new reclaim_t(data));
    }
    
    void delete_nodes_with_no_hazards(){
        reclaim_t* current = reclaim_head.exchange(nullptr);
        while(current){
            reclaim_t* const next=current->next;
            if(!unreleased_hazard_pointer(current->data)){
                delete current;
            }else{
                add_to_reclaim_list(current);
            }
            current = next;
        }
    }

public:
    lock_free_stack():
        head(nullptr) {
    }

    void push(const T &t){
        snode_t *neo = new snode_t(t);
        neo->next = head.load();

        while(head.compare_exchange_weak(neo->next, neo)){ } 
    }

    std::shared_ptr<T> pop(){
        std::atomic<void*>& hp = get_hazard_pointer_for_current_thread(); 
        snode_t *old_head = head.load(); 
        do{ 
            snode_t *temp; 
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
            if(unreleased_hazard_pointer(old_head)){
                reclaim_later(old_head);
            }else{ 
                delete old_head;
            }
            delete_nodes_with_no_hazards();
        }
        return res;
    }
};
