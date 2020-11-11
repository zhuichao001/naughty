#include "types.h"
#include <iostream>

using namespace std;


int main() {
    char buf[sizeof(Base)*128];
    Base *b = new(buf)Base(2);
    b->print();
    b->~Base();

    Base *bs = new(buf)Base(); //replacement new
    for(int i=0; i<128; ++i){
        new(bs+i)Base(i);
    }

    for(int i=119; i>=0; --i){
        (bs+i)->~Base();
    }
    return 0;
}


