#include <utility>
#include <stdio.h>

class B{
public:
    B(){}

    B(const B &b){
        printf("copy construct called.\n");
    }

    B(B &&b){
        printf("move construct called.\n");
    }

    B & operator=(const B & b){
        printf("copy assignment called.\n");
        return *this;
    }

    B & operator=(B && b){
        printf("move assignment called.\n");
        return *this;
    }
};

void foo(B && b){
    B first = b;
    printf("----------\n");
    B second = std::forward<B>(b);
    printf("==========\n");
    B third = std::move(b);
}

int main(){
    foo(B());
    return 0;
}
