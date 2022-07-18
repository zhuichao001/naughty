//#include "lock_free_stack.h"
//#include "lock_free_stack_v2.h"
#include "lock_free_stack_v3.h"
#include <iostream>
#include <vector>
#include <thread>

lock_free_stack<int> stack;

void op_stack(int id){
    for(int j=0; j<1000000; ++j){
        stack.push(id*1000000+j);
        std::shared_ptr<int> v = stack.pop();
        if(v==nullptr){
            std::cout<< "id:"  << id << " pop nullptr" << std::endl;
        }else{
            std::cout<< "id:"  << id << " pop " << *(v.get()) << std::endl;
        }
    }
}

void test(){
    std::vector<std::thread> threads;
    for(int  i=0; i<8; ++i){
        threads.emplace_back(op_stack, i);
    }

    for(int  i=0; i<8; ++i){
        threads[i].join();
    }
}

int main(){
    test();
    return 0;
}
