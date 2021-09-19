
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include "hlc/watch.h"

watch_t t(Precision::MILISECOND);
const int N = 1000*1000; //100w

void test_map(){
    std::unordered_map<std::string,std::string> strmap;

    int64_t start = t.now();
    for(int i=0; i<N; ++i){
        const std::string &key = std::to_string(i);
        const std::string &val = std::to_string(uint64_t(i)*i);
        strmap[key] = val;
    }
    int64_t end = t.now();
    std::cout << "std::map cost " <<  end-start << "ms\n";
}

void test_unordered_map(){
    std::map<std::string,std::string> strmap;

    int64_t start = t.now();
    for(int i=0; i<N; ++i){
        const std::string &key = std::to_string(i);
        const std::string &val = std::to_string(uint64_t(i)*i);
        strmap[key] = val;
    }
    int64_t end = t.now();
    std::cout << "std::unordered_map cost " <<  end-start << "ms\n";
}


int main () {
    test_map();
    test_unordered_map();

    return 0;
}
