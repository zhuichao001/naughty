
// 参考：https://selfboot.cn/2024/05/07/variadic_arguments_in_c++/

#include <iostream>

// 当只有一个参数时
template<typename T>
void  Print(T t) {
    std::cout << t << std::endl;
}

// 递归展开：接受一个参数和一个参数包，处理一个参数后，将剩余的参数包传递给下一个递归调用
template<typename T, typename... Args>
void Print(T first, Args... args) {
    std::cout << first << " ";
    Print(args...);
}

int main() {
    Print(1, 2, 3, 4, 5);
    return 0;
}
