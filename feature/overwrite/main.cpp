#include <iostream>
using namespace std;

class Base {
public:
    virtual void f(float x){ cout << "Base::f(float) " << x << endl; }
    virtual void f(int x){ cout << "Base::f(int) " << x << endl; }
};

class Derived : public Base {
public:
    virtual void f(char x) const { cout << "Derived::f(char) " << x << endl; }
};

int main() {
    Derived  d;
    Base *pb = &d;
    Derived *pd = &d;

    // Bad : behavior depends solely on type of the object
    pb->f(6); // Base::f(int)
    pb->f(3.14f); // Base::f(float) 3.14
    pd->f(3.14f); // Derived::f(float) 3.14

    return 0;
}

