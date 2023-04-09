#include <variant>
#include <string>
#include <iostream>

int main(){
    std::variant <int,double, std::string> va;
    va = 3.14; 
    std::cout << std::get<double>(va) << std::endl;

    va = 6;
    std::cout << std::get<0>(va) << std::endl;

    va = std::string("abc");
    std::cout << std::get<std::string>(va) << std::endl;
    return 0;
}
