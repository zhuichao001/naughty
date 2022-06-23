#include <vector>
#include <memory>
#include <iostream>

class X {
};

//Can't pass compile
void test(){
    std::vector<std::auto_ptr<X> > vec;
    vec.push_back(std::auto_ptr<X>(new X));
    std::auto_ptr<X> x = vec[0];  // vec[0] is assigned NULL.
}

int main(){
    test();
    return 0;
}
