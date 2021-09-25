#include <string>
#include <iostream>
#include "stdio.h"

void f(std::string &data){
    data.resize(10);
    data+="abc123...";
}

int main(){
    std::string a;
    f(a);
    fprintf(stderr, "%*.s, size:%d\n", a.size(), a.c_str(), a.size());
    std::cout<<a<<std::endl; //TODO: compare the print difference
    return 0;
}
