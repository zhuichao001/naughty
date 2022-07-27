#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <iostream>
#include <atomic>
#include <string>
#include <memory>


template<typename S, typename T>
struct node_t {
    struct node_link_t{
        node_t **forwards; // next for i'th layer
        node_link_t(int level){
            forwards = new node_t*[level];
            for (int i=0; i<level; ++i) {
                forwards[i] = nullptr;
            }
        }
    };

    S key;
    T val;
    uint64_t sn;
    int level;
    std::shared_ptr<node_link_t> link;
    node_t *next;  //link different version of same key

    node_t(uint64_t seqno, int lev, const S &k, const T &v=""):
        key(k),
        val(v),
        sn(seqno),
        level(lev),
        link(new node_link_t(lev)),
        next(nullptr) {
    }

    void print(){
        std::cout << "[node] key:" << key << std::endl;
        for(int i=0; i<level; ++i){
            std::cout << "    i:" << i << ", sibling:" << link->forwards[i] << std::endl;
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

    std::atomic<uint64_t> sn_line;

    skiplist_t(int maxh=16, int width=4):
        MAXHEIGHT(maxh),
        WIDTH(width),
        height(1),
        length(0) {
        head = new node_t<S,T>(0, MAXHEIGHT, "");
        nil = new node_t<S,T>(0, MAXHEIGHT, std::string(2048, '\xff'));  //FIXME
        for (int i=0; i<MAXHEIGHT; ++i) {
            head->link->forwards[i] = nil;
        }
    }

    ~skiplist_t(){
        clear(); //TODO
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

    node_t<S,T> *find(const std::string &k) {
        node_t<S,T> *cur = this->head;
        for (int i=this->height-1; i>=0; --i) {
            while (cur->link->forwards[i]->key<k) {
                cur = cur->link->forwards[i];
            }
            if (cur->link->forwards[i]->key==k) {
                return cur->link->forwards[i];
            }
        }
        return nullptr;
    }

    node_t<S,T> *insert(const std::string &k, const std::string &v){
        node_t<S,T> *update[this->MAXHEIGHT];
        node_t<S,T> *prev = this->head;
        node_t<S,T> *cur = this->head;
    
        for (int i=this->height-1; i>=0; --i) {
            while (cur->link->forwards[i]->key < k) {
                prev = cur;
                cur = cur->link->forwards[i];
            }
            update[i] = cur;
        }
    
        //update for same key
        if(cur->link->forwards[0]->key==k){
            node_t<S,T> * neo = new node_t<S,T>(0, cur->level, k, v); //FIXME
            neo->link = cur->link;
            for(int i=0; i<prev->level; ++i){
                if(prev->link->forwards[i]==cur){ //TODO atomic
                    prev->link->forwards[i] = neo;
                }
            }
            return neo;
        }

        //insert new key
        const int h = rand_level();
        node_t<S,T> * neo = new node_t<S,T>(0, h, k, v); //FIXME
        for (int i=this->height; i<h; ++i) {
            update[i] = this->head;
        }

        for (int i=0; i<h; ++i) {
            neo->link->forwards[i] = update[i]->link->forwards[i];
            update[i]->link->forwards[i] = neo;
        }
        this->height = std::max(this->height, h);
        ++this->length;
        return neo;
    }

    bool erase(const S &key) {
        node_t<S,T> *update[this->MAXHEIGHT];
        node_t<S,T> *cur = this->head;

        for (int i=this->height-1; i>=0; --i) {
            while (cur->link->forwards[i]->key < key) {
                cur = cur->link->forwards[i];
            }
            update[i] = cur;
        }
    
        if (cur->link->forwards[0]->key != key) { //update
            return false;
        }

        node_t<S,T> *dst = cur->link->forwards[0];
        for (int i=0; i<this->height; ++i) {
            if (update[i]->forward[i] != dst) {
                break;
            }
            update[i]->forward[i] = dst->forward[i];
        }

        delete dst;
        while (this->height>1 && this->head->forward[height-1]==nil) {
            --this->height;
        }
        --this->length;
    }

    void clear(){
        node_t<S, T> *cur = head->link->forwards[0];
        while(cur!=nil){
            node_t<S, T> *tmp = cur;
            cur = cur->link->forwards[0];
            delete tmp;
        }
        head->link->forwards[0] = nil;
    }

    void print(){
        std::cout << "level:" << this->height 
            << ",length:" << this->length << std::endl;
        for(int i=this->height-1; i>=0; --i){
            node_t<S,T> *cur = this->head;
            while(cur != this->nil){
                std::cout << cur->key << ":" << cur->val << " |-> ";
                cur = cur->link->forwards[i];
            }
            std::cout << "nil" << std::endl;
        }
    }
};
