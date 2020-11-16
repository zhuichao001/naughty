#include <iostream>
#include <functional>


void func(int x, int y) {
    std::cout << x << " " << y << std::endl;
}

int main() {
    //function adpter
    std::bind(func, std::placeholders::_1, 2)(1); //func(1,2)

    std::function<void(int, int)> fb = std::bind(func, std::placeholders::_1, std::placeholders::_2);
    fb(10, 20); //func(10,20);

    std::function<void(int, int)> fc = std::bind(func, std::placeholders::_2, std::placeholders::_1);
    fc(11, 21);// func(21,11)

    const int p=0;
    std::function<void(int, int)> fd = std::bind(func, p, std::placeholders::_2);
    fd(12, 22);// func(p,11)

    return 0;
}
