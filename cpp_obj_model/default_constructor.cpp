#include <stdio.h>

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
