#pragma once

#include <assert.h>
#include <thread>
#include <map>
#include <list>
#include <unordered_map>

struct runtime_info_t;
std::unordered_map<std::thread::id, runtime_info_t*> runtime_map;

struct runtime_info_t {
    std::thread::id tid;
    uint64_t sn;

    runtime_info_t():
        sn(0) {
        tid = std::this_thread::get_id();
    }

    void set_sn(uint64_t seqno){
        assert(seqno>sn);
        sn = seqno;
    }

    uint64_t get_sn(){
        return sn;
    }
};

runtime_info_t *create_runtime_info(){
    std::thread::id tid = std::this_thread::get_id();
    if(runtime_map.find(tid) == runtime_map.end()){
        runtime_info_t *res = new runtime_info_t();
        runtime_map[tid] = res;
        return res;
    }else{
        return runtime_map[tid];
    }
}

runtime_info_t *get_runtime_info(){
    thread_local static std::unique_ptr<runtime_info_t> ri(create_runtime_info());
    return ri.get();
}

////////////////////////////////////////////////////

uint64_t min_runtime_sn(){
    uint64_t min_sn = ~0;
    for(auto x: runtime_map){
        runtime_info_t *ri = x.second;
        uint64_t sn = ri->get_sn();
        if(sn!=0 && sn<min_sn){
            min_sn = sn;
        }
    }
    return min_sn;
}

uint64_t incr_global_sn(int by=0){
    static std::atomic<uint64_t> global_sn;
    if (by==0) {
        return ++global_sn;
    } else {
        return global_sn;
    }
}

void ebr_enter(int by=0){
    get_runtime_info()->set_sn(incr_global_sn(by));
}

void ebr_leave(){
    get_runtime_info()->set_sn(0);
}

enum class RW_TYPE{
    READ = 0,
    WRITE = 1,
};

class ebr_guard_t{
public:
    ebr_guard_t(RW_TYPE rwtype){
        if (rwtype==RW_TYPE::READ) {
            ebr_enter(0);
        } else {
            ebr_enter(1);
        }
    }
    ~ebr_guard_t(){
        ebr_leave();
    }
};
