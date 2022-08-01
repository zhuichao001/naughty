#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <iostream>
#include <atomic>
#include <string>
#include <memory>
#include "runtime_info.h"

enum ENTRY_TYPE{
    ENTRY_DATA = 0,
    ENTRY_DEL = 1,
};

template<typename S, typename T>
struct data_entry_t{
    const uint64_t sn;
    const ENTRY_TYPE kvtype;
    const S key;
    T val;
    data_entry_t<S,T>* next;

    data_entry_t(const uint64_t seqno, const ENTRY_TYPE type, const S &k, const T &v):
        sn(seqno),
        kvtype(type),
        key(k),
        val(v),
        next(nullptr){
    }
};

template<typename S, typename T>
struct node_t {
    const int level;
    std::atomic<data_entry_t<S,T> *> data;
    std::atomic<node_t<S,T> *> *forwards; // next for i'th layer

    node_t(const uint64_t seqno, const int lev, const ENTRY_TYPE type, const S &k, const T &v=""):
        level(lev),
        data(new struct data_entry_t<S,T>(seqno, type, k, v)) {
        forwards = new std::atomic<node_t<S,T>*>[level];
    }

    ~node_t(){
        data_entry_t<S,T> *ptr = data.load(std::memory_order_acquire);
        while(ptr!=nullptr){
            data_entry_t<S,T> *tmp = ptr;
            ptr = ptr->next;
            delete tmp;
        }
    }

    void print(){
        std::cout << "[node] key:" << data.load()->key << std::endl;
        for(int i=0; i<level; ++i){
            std::cout << "    i:" << i << ", sibling:" << forwards[i] << std::endl;
        }
    }
};

template<typename S, typename T>
struct skiplist_t {
    node_t<S,T> *head;
    node_t<S,T> *nil;

    const int MAXHEIGHT;
    const int WIDTH;

    int length;
    int height;

    skiplist_t(int maxh=16, int width=4):
        MAXHEIGHT(maxh),
        WIDTH(width),
        height(1),
        length(0) {
        head = new node_t<S,T>(0, MAXHEIGHT, ENTRY_DATA, "");
        nil = new node_t<S,T>(0, MAXHEIGHT, ENTRY_DATA, std::string(1024, '\xff'));
        for (int i=0; i<MAXHEIGHT; ++i) {
            head->forwards[i] = nil;
            nil->forwards[i] = nullptr;
        }
    }

    ~skiplist_t(){
        node_t<S, T> *cur = head->forwards[0];
        while (cur!=nil) {
            node_t<S, T> *tmp = cur;
            cur = cur->forwards[0];
            delete tmp;
        }
        delete head;
        delete nil;
    }

    int size(){ return length; }

    int rand_level() {
        int h = 1;
        while (h < MAXHEIGHT && rand()%WIDTH == 0) {
            ++h;
        }
        return h;
    }

    int get(const std::string &k, std::string &v) {
        ebr_guard_t g(RW_TYPE::READ);

        node_t<S,T> *prev = previous(0, k);
        if(prev==nullptr){
            return -1;
        }

        node_t<S,T> *cur = prev->forwards[0].load(std::memory_order_acquire);
        data_entry_t<S,T> *dat = cur->data.load(std::memory_order_acquire);
        if(dat->key==k && dat->kvtype==ENTRY_DATA){
            v = dat->val;
            return 0;
        }
        return -1;
    }

    void put(const std::string &key, const std::string &val){
        ebr_guard_t g(RW_TYPE::WRITE);

        insert(ENTRY_DATA, key, val);
    }

    void remove(const std::string &key){
        ebr_guard_t g(RW_TYPE::WRITE);

        insert(ENTRY_DEL, key, "");
    }

    void print(){
        std::cout << "level:" << this->height 
            << ",length:" << this->length << std::endl;
        for(int i=this->height-1; i>=0; --i){
            node_t<S,T> *cur = this->head;
            while(cur != this->nil){
                std::cout << cur->data.load()->key << ":" << cur->data.load()->val << " |-> ";
                cur = cur->forwards[i];
            }
            std::cout << "nil" << std::endl;
        }
    }

private:
    node_t<S,T> *previous(const int level, const std::string &k){
        assert(level<=this->height-1);
        node_t<S,T> *node = this->head;
        for (int i=this->height-1; i>=level; --i) {
            node_t<S,T> *tmp = node->forwards[i].load(std::memory_order_acquire);
            data_entry_t<S,T> *data = tmp->data.load(std::memory_order_acquire);
            while (data->key < k) {
                node = node->forwards[i];
            }
        }
        return node;
    }

    void insert(const ENTRY_TYPE type, const std::string &k, const std::string &v){
        uint64_t sn = get_runtime_info()->get_sn();

        node_t<S,T> *prev = previous(0, k);
        node_t<S,T> *cur = prev->forwards[0].load(std::memory_order_acquire);

        //exist same key
        if(cur->data.load(std::memory_order_acquire)->key==k){
            data_entry_t<S,T> *newdat = new data_entry_t<S,T>(sn, type, k, v);
            newdat->next = cur->data.load(std::memory_order_relaxed);
            while(!cur->data.compare_exchange_weak(newdat->next, newdat));
            return ;
        }

        //insert new key
        const int lev = rand_level();
        node_t<S,T> * neo = new node_t<S,T>(sn, lev, ENTRY_DATA, k, v); //FIXME
        neo->forwards[0] = cur;
        while(!prev->forwards[0].compare_exchange_weak(cur, neo)){
            neo->forwards[0] = cur;
        }

        for (int i=1; i<lev; ++i) {//build index
            node_t<S,T> *iprev = previous(i, k);
            node_t<S,T> *icur = iprev->forwards[0].load(std::memory_order_acquire);
            while(!iprev->forwards[i].compare_exchange_weak(icur, neo)){
                iprev->forwards[i] = icur;
            }
        }

        this->height = std::max(this->height, lev);
        ++this->length;
    }

};
