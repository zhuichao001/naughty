#include "placement_new.h"
#include <iostream>

using namespace std;


int main() {
    char buf[sizeof(Ping)*128];
    Ping *ping = new(buf)Ping(2);
    ping->print();
    ping->~Ping();

    Ping *pings = new(buf)Ping(); //replacement new
    for(int i=0; i<128; ++i){
        new(pings+i)Ping(i);
    }

    for(int i=119; i>=0; --i){
        (pings+i)->~Ping();
    }
    return 0;
}


