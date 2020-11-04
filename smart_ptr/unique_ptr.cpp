#include <iostream>
#include <memory>


void reset_unique_ptr(){
    std::unique_ptr<int> up;  // empty

    up.reset (new int);       // takes ownership of pointer
    *up=5;
    std::cout << *up << '\n';

    up.reset (new int);       // deletes managed object, acquires new pointer
    *up=10;
    std::cout << *up << '\n';

    up.reset();               // deletes managed object
}

void release_unique_ptr(){
    std::unique_ptr<int> up(new int);
    *up=10;

    int *mp = up.release(); // (auto_pointer is now empty)
    std::cout << "after release, mp points to " << *mp << '\n';
    delete mp;

}

void move_unique_ptr(){
    std::unique_ptr<int> up1(new int(11));
    std::cout << *up1 << std::endl;

    //unique_ptr<int> up2 = up1;            // error, can not be compiled
    std::unique_ptr<int> up3 = std::move(up1);
    std::cout << *up3 << std::endl;

    //std::cout << *up1 << std::endl;   // error, runtime err
    up3.reset();
    up1.reset();
}

int main () {
    reset_unique_ptr();
    release_unique_ptr();
    move_unique_ptr();
    return 0;
}
