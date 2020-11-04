#include "singleton.h"
#include <iostream>

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

int main(){
    Spec a;
    Spec b;
    a.instance().show();
    b.instance().show();
    return 0;
}
