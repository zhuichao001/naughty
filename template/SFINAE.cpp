//SFINAE: Substitution failure is not an error （ 替换失败不是错误）

#include <iostream>
#include <vector>
#include <string>

struct A {
    int x;
    int size() { return x; }
};

struct B {
    int y;
    int len() { return y; }
};

template<typename T>
void foo(const T & t, decltype(T::x) *ptr=nullptr) {
    std::cout << "type T have member x" << std::endl;
}

template<typename T>
void foo(const T& t, decltype(T::y)* ptr=nullptr) {
    std::cout << "type T have member y" << std::endl;
}

void test_ok(){
    A a{ 101 };
    foo(a);

    B b{ 216 };
    foo(b);

    struct C {
        int x, a, b;
    };
    C c{ 1, 3, 9 };
    foo(c);
}

////////////////////////////////////////////////

/*
void test_fail(){
    struct D {
        int x, y, b;
    };
    D d{ 1, 5, 9 };
    foo(d); //error: call of overloaded ‘foo(test_fail()::D&)’ is ambiguous
}
*/

////////////////////////////////////////////////
template<typename T>
auto foo(T& t) -> decltype(std::declval<T>().size()) {
    std::cout << "type T have member function ::size()" << std::endl;
    return t.size();
}

void test_2(){
    A a{ 101 };
    foo(a);

    std::vector<int>v = { 1, 2, 3, 4, 5};
    std::cout << foo(v) << std::endl;
}

////////////////////////////////////////////////
template<typename T>
void foo(T t, typename std::enable_if<std::is_same<int, decltype(T::len)>::value, int>::type* ptr = nullptr) {
    std::cout << "type T have member: int len" << std::endl;
}

void test_3(){
    struct C{
        int len;
    };
    struct D {
        std::vector<int> v;
        std::string s;
        int len;
    };

    C c{333};
    D d{};
    foo(c);
    foo(d);
}

int main() {
    //test_ok();
    //test_fail();
    //test_2();
    test_3();
    return 0;
}
