#pragma once

#include "runtime_info.h"

struct snapshot_t;
std::list<snapshot_t*> snapshot_list;

struct snapshot_t {
    uint64_t sn;
    snapshot_t(uint64_t seqno):
        sn(seqno) {
    }
};

snapshot_t *create_snapshot(){
    uint64_t sn = incr_global_sn(0);
    snapshot_t * ss = new snapshot_t(sn);
    snapshot_list.push_back(ss);
    return ss;
}
