#include <iostream>
#include <functional>

void fun(int& _a, int& _b, int _c)
{
    _a++;
    _b++;
    _c++; 
    std::cout << "in fun a:" << _a << " b:" << _b << " c:" << _c << std::endl;
}

int main()
{
    int a = 1, b = 1, c = 1;
    auto b_fun = std::bind(fun, a, std::ref(b), std::ref(c));
    b_fun();

    std::cout << "after fun a:" << a << " b:" << b << " c:" << c << std::endl;
    return 0;
}

/* output:
 * in fun a:2 b:2 c:2
 * after fun a:1 b:2 c:1
 * */
