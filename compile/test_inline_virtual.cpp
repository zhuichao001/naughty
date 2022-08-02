#include <iostream>
using namespace std;

class Base {
public:
    inline virtual void Display() {
        cout << "Base: Display" << endl;
    }
};

class Derived : public Base {
public:
    inline virtual void Display() {
        cout << "Derived: Display" << endl;
    }
};

int main() {
    // Virtual functions are called by concrete objects and can be inline 
    Base b;
    b.Display();

    // The virtual function here is called through the base class pointer and cannot be inline
    Base* bp = new Derived();
    bp->Display();
    delete bp;
    bp = nullptr;

    return 0;
}
