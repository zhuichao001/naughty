#include <stdio.h>

class Widget{
public:
    virtual void sound() = 0;
};

class Bell: public Widget{
    void sound() override {printf("Bell sound.\n");}
};

class Whistle: public Widget{
    void sound() override {printf("Whistle sound.\n");}
};

void sound(Widget & w){w.sound();}

/*
 * 编译器会(在constructor中)安插一些代码以便为每一个 Widget（或其派生类的） object的vptr设定初值，放置适当的virtual table地址;
 * 对于那些未声明任何constructors的classes，编译器会为它们合成一个default constructor，以便正确地初始化每一个class object的vptr.
 * */
int main(){
    Bell b;
    Whistle t;
    sound(b);
    sound(t);
    return 0;
}
