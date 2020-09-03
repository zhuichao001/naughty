#include "placement_new.h"
#include <iostream>

using namespace std;


//reference: www.geeksforgeeks.org/placement-new-operator-cpp/

//void* operator new(std::size_t) throw(std::bad_alloc);      // normal new
//void* operator new(std::size_t, void*) throw();             // placement new
//void* operator new(std::size_t, const std::nothrow_t&) throw();     // 见 Item 49


int main() {
	char buf[sizeof(Ping)*128];
	Ping *ping = new(buf)Ping(2);
	ping->print();
	ping->~Ping();

	Ping *pings = new(buf)Ping();
    for(int i=0; i<128; ++i){
		new(pings+i)Ping(i);
    }

    for(int i=119; i>=0; --i){
		(pings+i)->~Ping();
    }
	return 0;
}
