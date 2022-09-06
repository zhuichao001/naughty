#include<iostream>

class Base{
};

class Father:public virtual Base{
}; 

class Mother:public virtual Base{
};

class Son:public Father,public Mother{
};

int main(){
    Base *b;
    std::cout<<"sizeof Base:"<<sizeof(Base)
        <<"sizeof BasePtr:"<<sizeof(b)
        <<",sizeof Father:"<<sizeof(Father)
        <<",sizeof Mother:"<<sizeof(Mother)
        <<",sizeof Son:"<<sizeof(Son)<<std::endl;
    return 0;
}
