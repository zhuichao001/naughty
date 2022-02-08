#include<iostream>
using namespace std;

class Base{
protected:
    int value;
public:
    Base()=default;
    Base(int v):
        value(v){
        std::cout<< "Base Called" <<std::endl;
    }
};

class Father:public Base{
public:
    Father()=default;
    Father(int v):
        Base(v){
        std::cout<< "Father Called" <<std::endl;
    }

    void set_value(int value){
        this->value=value;
    }
}; 

class Mother:public Base{
public:
    Mother()=default;
    Mother(int v):
        Base(v){
        std::cout<< "Mother Called" <<std::endl;
    }
    int get_value(){
        return this->value;
    }
};

class Son:public Father,public Mother{
public:
    Son()=default;
    Son(int v):
        Father(v),
        Mother(v){
        std::cout<< "Son Called" <<std::endl;
    }
};

int main(){
    Son s(10);
    s.set_value(20);
    cout<<s.get_value()<<endl;
    return 0;
}
