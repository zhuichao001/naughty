#include <iostream>
#include <vector>
#include <initializer_list>

template <class T>
struct Vector {
    std::vector<T> v;

    S(std::initializer_list<T> l) : v(l) {
        std::cout << "constructed with a " << l.size() << "-element list\n";
    }

    void append(std::initializer_list<T> l) {
        v.insert(v.end(), l.begin(), l.end());
    }

    std::pair<const T*, std::size_t> c_arr() const {
        return {&v[0], v.size()};
    }
};

template <typename T>
void templated_fn(T) {}

int main(){
    Vector<int> s = {1, 2, 3, 4, 5};
    s.append({6, 7, 8});
    std::cout << "The vector size is now " << s.c_arr().second << " ints:\n";

    for (auto n : s.v)
        std::cout << n << ' ';
    std::cout << '\n';
    std::cout << "Range-for over brace-init-list: \n";


    auto al = {10, 11, 12};
    std::cout << "The list bound to auto has size() = " << al.size() << '\n';

    templated_fn<std::initializer_list<int>>({1, 2, 3});
    templated_fn<std::vector<int>>({1, 2, 3});
}

