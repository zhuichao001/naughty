#include <stdio.h>

class X{
public:
    char c;
};

class Y: public virtual X{
};

class Z: public virtual X{
};

class W: public virtual Y, public virtual Z {
};

int main(){
    printf("size of X:%d\n", sizeof(X));
    printf("size of Y:%d\n", sizeof(Y));
    printf("size of Z:%d\n", sizeof(Z));
    printf("size of W:%d\n", sizeof(W));
    return 0;
}
