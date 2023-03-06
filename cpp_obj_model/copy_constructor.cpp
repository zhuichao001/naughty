#include <string>
#include <stdio.h>

class Foo {
public:
    Foo(int i, std::string s):
        id(i),
        name(s){
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
    Foo back = foo;
    back.print();
    return 0;
}
