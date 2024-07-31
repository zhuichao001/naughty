// weak_ptr::operator= example
#include <iostream>
#include <memory>

int main () {
    std::shared_ptr<int> sp1,sp2;
    std::weak_ptr<int> wp;
    // sharing group:
    // --------------
    sp1 = std::make_shared<int> (10);    // sp1
    wp = sp1;                            // sp1, wp

    sp2 = wp.lock();                     // sp1, wp, sp2
    sp1.reset();                         //      wp, sp2

    //通过lock保留的临时指针，重新获取到了shared_ptr共享的地址
    sp1 = wp.lock();                     // sp1, wp, sp2

    std::cout << "*sp1: " << *sp1 << "|" << sp1.use_count() <<'\n';
    std::cout << "*sp2: " << *sp2 << "|" << sp2.use_count() <<'\n';

    return 0;
}
