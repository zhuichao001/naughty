#include <iostream>

using namespace std;


struct Base {
    Base(){type=0; cout<<"default construct, type:"<<type<<endl;}
    Base(int t):type(t){cout<<"parameter construct, type:"<<type<<endl;}
    ~Base(){cout<<"destroy, type:"<<type<<endl;}
    void print(){cout<<"type:"<<type<<endl;}
    int type;
};
