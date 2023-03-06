#include <stdio.h>
#include <string>

class Foo {
public:
    Foo(){
        printf("default Foo construct called.\n");
    }
};

class Bar{
    Foo foo;
    const std::string name;
    char *ptr;
public:
    Bar(const std::string &s):
        name(s){
        // foo::Foo() will be called. Compiler add implicit call
        printf("before ptr is set.\n");
        ptr = nullptr;
        printf("apfter ptr is set.\n");
    }
};

int main(){
    Bar bar(std::string("abc"));
    return 0;
}
