#include <iostream>
#include "hlc.h"


int main(){
    hlc_t h(Precision::SECOND);
    vtime_t t = h.now();
    std::cout << t.time << ":" << t.seq << std::endl;
    t = h.now();
    std::cout << t.time << ":" << t.seq << std::endl;
    t = h.now();
    std::cout << t.time << ":" << t.seq << std::endl;
    return 0;
}
