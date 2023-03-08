#include <iostream>
#include <functional>

using  CalcFun = std::function<int(int, int)>;

//抽象二元运算
int AbsDual(int a, int b, const CalcFun &func) {
    return func(a, b);
}

int Add(int a, int b) {
    return a + b;
}

int main() {
    int a = 4, b = 6;
    {
        int res = AbsDual(a, b, Add);
        std::cout << "Value: " << res << std::endl;
    }

    {
        CalcFun fun = std::bind(&Add, std::placeholders::_1, std::placeholders::_2);
        int res = AbsDual(a, b, fun);
        std::cout << "Value: " << res << std::endl;
    }
    return 0;
}
