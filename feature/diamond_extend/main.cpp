#include<iostream>
using namespace std;

class Base{
protected:
    int value;
public:
    Base()=default;
    Base(int v):value(v){}
};

class Father:public Base{
public:
    Father()=default;
    Father(int v):Base(v){}
    void set_value(int value){
        this->value=value;
    }
}; 

class Mother:public Base{
public:
    Mother()=default;
    Mother(int v):Base(v){}
    int get_value(){
        return this->value;
    }
};

class Son:public Father,public Mother{
public:
    Son()=default;
    Son(int v):Father(v),Mother(v){}
};

int main(){
    Son s(10);
    s.set_value(20);
    cout<<s.get_value()<<endl;
    return 0;
}
