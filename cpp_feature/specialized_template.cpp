#include <iostream>

template <class T>
class Comparable{
public:
    bool Equal(const T &s, const T &t);
};

template<class T>
bool Comparable<T>::Equal(const T &s, const T &t){
    return (s==t);
}

template<>
class Comparable<double>{
public:
    bool Equal(const double &s, const double &t);
};

//模板特化以后,实际上其本身已经不是templatized, 所以不使用template模板说明
bool Comparable<double>::Equal(const double&s, const double &t){
    std::cout << "Call Comparable<double>::Equal" <<std::endl;
    return abs(s-t) < 10e-3;
}

void test1(){
    Comparable<int> intcmp;
    std::cout << intcmp.Equal(2,2) << std::endl;
    std::cout << intcmp.Equal(4,4) << std::endl;

    Comparable<double> doublecmp;
    std::cout << doublecmp.Equal(3.14159, 3.14159) << std::endl;
}

int main(){
    test1();
    return 0;
}
