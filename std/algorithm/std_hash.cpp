#include <iostream>
#include <functional>
#include <string>

int main() {
    std::hash<int> hash_func;
    {
        size_t str_hash = hash_func(1);
        std::cout << str_hash << '\n';
    }

    {
        size_t str_hash = hash_func(2);
        std::cout << str_hash << '\n';
    }
    return 0;
}
