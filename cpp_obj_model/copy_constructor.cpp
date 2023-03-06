#include <string>
#include <stdio.h>

class Foo {
public:
    Foo(){
        printf("default constructor: Foo()\n");
    }

    Foo(int i, std::string s):
        id(i),
        name(s){
        printf("constructor: Foo(int, std::string)\n");
    }

    Foo(const Foo &f){
        printf("copy constructor\n");
        id = f.id;
        name = f.name;
    }

    Foo & operator=(const Foo &f){
        if(this==&f){
            return *this;
        }
        printf("copy assignment\n");
        id = f.id;
        name = f.name;
        return *this;
    }

    void print(){
        printf("id:%d, name:%s\n", id, name.c_str());
    }
private:
    int id;
    std::string name;
};

int main(){
    Foo foo(123,"abc");
    Foo back = foo; //copy constructor
    back.print();
    return 0;
}
