#include <iostream>
#include <type_traits>

// 检查类是否具有名为 `value` 的成员变量
template<typename T, typename = void>
struct has_value_member : std::false_type {};

template<typename T>
struct has_value_member<T, std::void_t<decltype(std::declval<T>().value)>> : std::true_type {};

// 示例类
struct A {
    int value;
};

struct B {
    float other_value;
};

int main() {
    std::cout << std::boolalpha;
    std::cout << "A has value member: " << has_value_member<A>::value << std::endl; // 输出 true
    std::cout << "B has value member: " << has_value_member<B>::value << std::endl; // 输出 false
    return 0;
}
