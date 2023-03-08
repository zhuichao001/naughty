#include <iostream>

typedef int (*CalcFun)(int, int);

//抽象二元运算
int AbsDual(int a, int b, const CalcFun &func) {
    return func(a, b);
}

int Add(int a, int b) {
    return a + b;
}

int main() {
    int a = 4, b = 6;
    int res = AbsDual(a, b, Add);

    std::cout << "Value: " << res << std::endl;
    return 0;
}
