#include <iostream>

template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

int main() {
    constexpr int f5 = Factorial<5>::value;
    constexpr int f1 = Factorial<1>::value;

    std::cout << "Factorial<5>::value: " << f5 << std::endl; // 输出 120
    std::cout << "Factorial<0>::value: " << f0 << std::endl; // 输出 1

    return 0;
}
