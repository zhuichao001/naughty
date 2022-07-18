//yum install libatomic1 libatomic
//g++ [this cpp file] -latomic
#include <iostream>
#include <atomic>

struct A { int a[100]; };
struct B { int x, y; };

int main() {
    std::atomic<A> a;
    std::atomic<B> b;
    std::cout << std::boolalpha
        << "std::atomic<A> is lock free? "
        << std::atomic_is_lock_free(&a) << '\n'
        << "std::atomic<B> is lock free? "
        << std::atomic_is_lock_free(&b) << '\n';
    return 0;
}
