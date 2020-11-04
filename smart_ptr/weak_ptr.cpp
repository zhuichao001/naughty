#include <iostream>
#include <memory>

void lock_weak_ptr(){
    std::shared_ptr<int> sp1,sp2;
    std::weak_ptr<int> wp;

                                         // sharing group:--------------
    sp1 = std::make_shared<int> (20);    // sp1    null,  null
    wp = sp1;                            // sp1,   wp,    null

    sp2 = wp.lock();                     // sp1,   wp,    sp2
    sp1.reset();                         // null,  wp,    sp2

    sp1 = wp.lock();                     // sp1,   wp,    sp2

    std::cout << "*sp1: " << *sp1 << '\n';
    std::cout << "*sp2: " << *sp2 << '\n';
}

int main () {
    lock_weak_ptr();
    return 0;
}
