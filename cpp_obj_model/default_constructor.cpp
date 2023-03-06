#include <stdio.h>

//编译器帮默认构造函数中将会调用成员变量的默认构造函数
class Foo {
public:
    Foo(){
        printf("default Foo construct called.\n");
    }
};

class Bar{
    Foo foo;
    char *ptr;
public:
    Bar(){
        // foo::Foo() will be called. Compiler add implicit call
        ptr = nullptr;
    }
};

int main(){
    Bar bar;
    return 0;
}
