#include <iostream>
#include <vector>
#include <thread>
#include <unistd.h>
#include "blocking_queue.h"

BlockingQueue<int> bq(32);

void produce(int tid, int start, int end){
    for(int i=start; i<end; ++i){
        bq.push(i);
        std::cout << "producer-" << tid << " push " << i << std::endl;
        usleep(1);
    }
}

void consume(int tid){
    while(true){
        int v = bq.pop();
        std::cout << "consumer-"<< tid << " pop " << v << std::endl;
        usleep(1);
    }
}

int main(){
    const int N = 8;
    std::vector< std::thread * > producers(N);
    std::vector< std::thread * > consumers(N);
    
    const int Count = 1000;
    for(int i=0; i<N; ++i){
        producers[i] = new std::thread(produce, i, i*Count, (i+1)*Count);
        consumers[i] = new std::thread(consume, i);
    }

    for(int i=0; i<N; ++i){
        producers[i]->join();
    }
    sleep(3);
}
