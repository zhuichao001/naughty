#include <stdio.h>

//编译器按顺序在其默认构造函数中将会调用成员变量的默认构造函数

class Desk {
public:
    Desk(){
        printf("default Desk construct called.\n");
    }
};

class Picture {
public:
    int pid;
};

class Chair {
public:
    Chair(){
        printf("default Chair construct called.\n");
    }
};

class Room{
    Desk desk;
    Picture pic;
    Chair chair;
    char *ptr;
public:
    Room(){
        //desk::Desk();
        //no Picture default will be called
        //chair::Chair();
        ptr = nullptr;
    }
};

int main(){
    Room room;
    return 0;
}
