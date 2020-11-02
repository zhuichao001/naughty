#include <iostream>

using namespace std;


class Base{
  public:
    virtual void Read(char c){
        cout << "Base::Read " << c << endl;
    }

    void Write(char c){
        cout << "Base::Write " << c << endl;
    }
};


class Derived : public Base{
  public:
    virtual void Read(char c){
        cout << "Derived::Read " << c << endl;
    }

    int Write(char c){
        cout << "Derived::Write " << c << endl;
        return 0;
    }
};


int main(){
    Base *b;
    Derived d;
    b = &d;
    b->Read('a'); //Derived::Read a
    b->Write('b'); //Base::Write b
    return 0;
}
