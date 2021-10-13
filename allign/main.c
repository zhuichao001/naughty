#include <stdio.h>

struct A {
    int i;
    char c;
    short d;
};

struct B {
    char c;
    int i;
    short d;
};

struct Ai {
    // int i;
    char c;
    short d;
};

struct Bi {
    char c;
    // int i;
    short d;
}; 

#pragma pack(2)
struct C {
    char c;
    int i;
    short d;
};
#pragma pack()

#pragma pack(1)
struct D {
    char c;
    int i;
    short d;
};
#pragma pack()

void test(){
    printf("sizeof A:%d\n", sizeof(struct A));
    printf("sizeof B:%d\n", sizeof(struct B));
    printf("sizeof Ai:%d\n", sizeof(struct Ai));
    printf("sizeof Bi:%d\n", sizeof(struct Bi));
    printf("sizeof C:%d\n", sizeof(struct C));
    printf("sizeof D:%d\n", sizeof(struct D));
}

int main() {
    test();
    return 0;
}
