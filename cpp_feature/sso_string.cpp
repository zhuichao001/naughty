#include<string>
#include<iostream>

int main(){
    std::string s = "abcefg";
    std::cout << &s << " " << (void *)(s.c_str()) << std::endl;
    return 0;
}
