#include "singleton.h"
#include <iostream>
#include <thread>

using namespace std;

class Spec: public singleton<Spec>{
public:
    Spec(){
        cout << "construct Spec" << endl;
    }

    ~Spec(){
        cout << "destruct Spec~" << endl;
    }

    void show(){
        cout << "method do() is called, ptr="<<this << endl;
    }

};

void foo(){
    Spec::instance().show();
}

int main(){
    std::thread t1(foo);
    std::thread t2(foo);
    t1.join();
    t2.join();
    return 0;
}
