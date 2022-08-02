#include <memory.h>
#include <iostream>

class Thing {
public:
    Thing(){
        data = new double[1000];
        memset(data, 0, sizeof(double)*1000);
        std::cout << "Construct" << std::endl;
    }

    Thing(const Thing &val){
        memcpy(data, val.data, sizeof(double) * 1000);
        std::cout << "Copy Construct" << std::endl;
    }

    Thing(Thing &&val){
        data = val.data;
        val.data = nullptr;
        std::cout << "Move Copy Construct" << std::endl;
    }

    Thing& operator=(const Thing &val){
        if (this != &val){
            memcpy(data, val.data, sizeof(double) * 1000);
        }
        std::cout << "Operator =" << std::endl;
        return *this;
    }

    Thing& operator=(Thing &&val){
        if (this != &val){
            data = val.data;
            val.data = nullptr;
        }
        std::cout << "Move Operator =" << std::endl;
        return *this;
    }

    ~Thing(){
        delete data;
        std::cout << "Destructor" << std::endl;
    }

public:
    double *data;
};

Thing getOne() {
    Thing t;
    t.data[0] = 100;
    return t;
}

Thing getRvalOne() {
    Thing t;
    t.data[0] = 100;
    return std::move(t); //complier will not RVO
}

//forced close RVO: -fno-elide-constructors
void test1(){
    Thing a = getOne();
}

void test2(){
    Thing a;
    a = getOne();
}

void test3(){
    Thing a = getRvalOne();
}

int main(int argc, char* argv[]) {  
    test3();
    return 0;
}
