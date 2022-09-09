#include <urcu-qsbr.h>
#include <pthread.h>
#include <poll.h>

const int N = 60000;

struct node{
    int key;
    int val;
    struct node *next;

    node(int k=0, int v=0, struct node *ptr=nullptr):
        key(k),
        val(v),
        next(ptr){
    }
} head;

int list_query(const int k, int &v){
    int err = -1;
    v = -1;

    rcu_read_lock();
    struct node *cur = head.next;
    while(cur!=nullptr){
        if(cur->key==k){
            v = cur->val;
            err = 0;
            break;
        }
        cur = cur->next;
    }
    rcu_read_unlock();

    return err;
}

int list_modify(const int k, const int v){
    int err = -1;

    struct node *cur = &head;
    struct node *neo = new struct node(k,v,nullptr);
    struct node *old = nullptr;

    rcu_read_lock();
    while(cur->next!=nullptr){
        if(cur->next->key==k){
            neo->next = cur->next->next;
            old = cur->next;
            err = 0;
            break;
        }
        cur = cur->next;
    }
    rcu_read_unlock();

    if(err==0){
        synchronize_rcu();
        cur->next = neo;
        delete old;
    }

    return err;
}

void *run_list_query(void *arg){
    rcu_thread_online();

    for(int i=0; i<N; ++i){
        int k=i;
        int v;
        list_query(i, v);
        if(i%100==0){
            rcu_quiescent_state();
        }
    }

    rcu_thread_offline();
    return nullptr;
}

void *run_list_modify(void *arg){
    rcu_thread_online();

    for(int i=0; i<1000; ++i){
        int k=i*60;
        int v=i*1000;
        list_modify(k, v);
        poll(nullptr, 0, 10);
    }

    rcu_thread_offline();
    return nullptr;
}

void init_list(){
    struct node *tail = &head;
    for(int i=0; i<N; ++i){
        tail->next = new struct node(i, i, nullptr);
        tail = tail->next;
    }
}

int main(){
    init_list();

    const int N_READER = 2;
    const int N_UPDATER = 1;
    pthread_t pid_reader[N_READER];
    pthread_t pid_updater[N_UPDATER];

    for(int i=0; i<N_READER; ++i){
        pthread_create(&pid_reader[i], NULL, run_list_query, NULL);
    }
    for(int i=0; i<N_UPDATER; ++i){
        pthread_create(&pid_updater[i], NULL, run_list_modify, NULL);
    }

    for(int i=0; i<N_READER; ++i){
        pthread_join(pid_reader[i], NULL);
    }
    for(int i=0; i<N_UPDATER; ++i){
        pthread_join(pid_updater[i], NULL);
    }
    return 0;
}
