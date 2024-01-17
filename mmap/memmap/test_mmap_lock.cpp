#include "mem_map.h"
#include <stdio.h>

//dd if=/dev/zero of=20m.dat bs=1M count=20
//
void test_lock_inmem(){
    Memmap m;
    m.load("./20m.dat", MAP_PRIVATE|MAP_POPULATE|MAP_LOCKED);

    madvise(m.data(), m.size(), MADV_WILLNEED|MAP_POPULATE|MAP_NONBLOCK);

    //char *buf = new char[20<<20];
    //m.read(0, buf, 20<<20);
}

void test_lock_notmem(){
    Memmap m;
    m.load("./20m.dat", MAP_SHARED|MAP_LOCKED);
}

int main(){
    //test_lock_notmem();
    test_lock_inmem();
    getchar();
    return 0;
}
