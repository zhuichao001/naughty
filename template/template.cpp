
#include <iostream>

template<typename... Ts>
void arglen(Ts... args) {
    std::cout << sizeof...(args) << std::endl;
}

void test1(){
    arglen();   // print 0
    arglen(1);  // print 1
    arglen(1, ""); // print 2
}

//---------------------------
template<typename T0>
void print(T0 value) {
        std::cout << value << std::endl;
}

template<typename T, typename... Ts>
void print(T value, Ts... args) {
    std::cout << value << " ";
    print(args...);
}

void test2(){
    print("123", 2, 3.14159, 'c');
}

int main(){
    test1();
    test2();
    return 0;
}
