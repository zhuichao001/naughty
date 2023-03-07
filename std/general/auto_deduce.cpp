#include <iostream>

int main(){
    auto x = 27;
    const auto cx = x;
    const auto & rx=cx;

    auto&& uref1 = x;               //x是int左值， 所以uref1类型为int&
    auto&& uref2 = cx;              //cx是const int左值， 所以uref2类型为const int&
    auto&& uref3 = 27;              //27是int右值， 所以uref3类型为int&&


    auto x1 = 27;                   //类型是int，值是27
    auto x2(27);                    //同上
    auto x3 = { 27 };               //类型是std::initializer_list<int>，值是{ 27 }
    auto x4{ 27 };                  //同上

    return 0;
}
