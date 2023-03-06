// 计算 N 的阶乘 N!

#include <iostream>
 
template<int N>
class Factorial{
public:
    enum { val = N * Factorial<N-1>::val };
};

template<>
class Factorial<0>{
public:
    enum { val = 1 };
};

int main(){
    std::cout << Factorial<10>::val << '\n';
    return 0;
}
