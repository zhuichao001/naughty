#include <iostream>

template<typename T>
class Storm{
public: 
    typedef const T Retype;
};

void f() { 
    std::cout << "global f()\n"; 
}

template<typename T>
class Base {
public:
    template <int N = 99>
    void f() { std::cout << "member f(): " << N << '\n'; }
};

template<typename T>
class Derived : public Base<T> {
    public:
        typename T::Retype m; // typename 不能省略
        Derived(typename T::Retype a) : m(a) { }
        void func1() { f(); }                       // 调用全局 f()，而非想象中的基类 f()
        void func2() { this->template f(); }        // 子类 f<99>()
        void func3() { Base<T>::template f<22>(); } // 强制基类 f<22>()
        void func4() { ::f(); }                     // 强制全局 f()
};

int main(){
    Derived<Storm<int>> a(10);
    a.func1(); 
    a.func2(); 
    a.func3(); 
    a.func4();
    return 0;
}
