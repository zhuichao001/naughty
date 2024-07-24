
// 参考：https://selfboot.cn/2024/05/07/variadic_arguments_in_c++/

#include <iostream>

// 变参的 sum 函数使用折叠表达式实现
template<typename... Args>
auto sum(Args... args) -> decltype((args + ...)) {
    return (args + ...);
}

// 变参的打印日志函数
template<typename... Args>
void show(Args&&... args) {
    (std::cout << ... << args) << '\n';  // C++17 折叠表达式
}

int main() {
    // 调用 sum 函数
    auto result = sum(1, 2, 3, 4, 5);
    std::cout << "The result is: " << result << std::endl;

    // 可以处理不同类型的数据，结果类型会根据传入参数自动推导
    auto result2 = sum(1, 2.5, 3.0, 4.5);
    std::cout << "The result with mixed types is: " << result2 << std::endl;

    // 类型安全检查会发现这里有问题，编译不过
    // auto result2 = sum(1, "2.5", 3.0, 4.5);

    show("This is a", " variadic", " template", " with", " folding", " expression.", 123, 45.67);
    return 0;
}
