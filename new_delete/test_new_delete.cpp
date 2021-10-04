#include <stdlib.h>
#include <iostream>

class Proto {
public:
    Proto() { std::cout << "Constructing" << std::endl; }
    ~Proto() { std::cout << "Destructing" << std::endl; }
};

//normal
void test1(){
    Proto *p = new Proto();
    delete p;

    Proto *pa = new Proto[2];
    delete []pa;
}

//new [] / delete
void test2(){
    Proto *pa = new Proto[2];
    delete pa;
}

//new / delete []
void test3(){
    Proto *p = new Proto();
    delete []p;
}

//new int / delete []int
//new int[] / delete
void test4(){
    int *p = new int(5);
    delete []p;

    int *pa = new int[4];
    delete pa;
}

int main(int argc, char *argv[]) {
    test1(); //ok
    //test2(); //abort
    //test3();  //abort
    test4();  //ok
    return 0;
}
