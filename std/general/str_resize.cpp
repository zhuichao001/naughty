#include <string>
#include <iostream>

void f(std::string &data){
    data.resize(10);
    data+="abc123";
}

int main(){
    std::string a;
    f(a);
    std::cout<<a<<std::endl;
    return 0;
}
