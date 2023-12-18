#include<iostream>

constexpr std::size_t factorial(std::size_t N) {
    std::size_t result = 1;
    for (std::size_t i = 1; i <= N; ++i)
    {
        result *= i;
    }
    return result;
}

int main() {
    constexpr auto a = factorial(5); // 编译期计算
    std::size_t& n = *new std::size_t(6);
    auto b = factorial(n); // 运行期计算
    std::cout << a << std::endl;
    std::cout << b << std::endl;
}
