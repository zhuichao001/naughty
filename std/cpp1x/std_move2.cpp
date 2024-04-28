#include<iostream>
#include<string.h>

class Member{
public:
    Member(const char *s){
        p_ = new char[strlen(s)+1];
        memcpy(p_,s,strlen(s)+1);
    }

    Member(const Member &m){
        p_ = new char[strlen(m.p_)+1];
        memcpy(p_, m.p_, strlen(m.p_)+1);
        std::cout << "copy-construct" <<std::endl;

    }

    Member(Member &&m){
        p_ = m.p_;
        m.p_=nullptr;
        std::cout << "move-construct" <<std::endl;
    }

char *p_;
};

class A{
public:
    A(Member m):m_(std::move(m)){
    }

    void print(){
        std::cout << m_.p_ <<std::endl;
    }

    Member m_;
};

int main(){
    Member m("hello");
    A a(m);
    a.print();
    return 0;
}
