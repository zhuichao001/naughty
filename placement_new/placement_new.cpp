#include "types.h"
#include <iostream>

using namespace std;

void test1(){
    char buf[sizeof(Base)*1];
    Base *b = ::new(buf) Base(2);
    b->print();
    b->~Base();
}

void test2(){
    const int N = 128;
    char *buff = static_cast<char*>(::operator new(sizeof(Base)*N));
    Base *bs = ::new(buff+0) Base(); //replacement new
    for(int i=0; i<N; ++i){
        ::new(bs+i) Base(i);
    }

    for(int i=N-1; i>=0; --i){
        (bs+i)->~Base();
    }
}

void test3(){
    Base *b = new Base(333);
    delete b;
}

int main() {
    //test1();
    //test2();
    test3();
    return 0;
}
