#include <iostream>

template<typename... Args>
auto SumWith(Args... args){
        return (0 + ... + args);
}

int main(){
    std::cout << SumWith(1,2,3,4) << std::endl;
}
