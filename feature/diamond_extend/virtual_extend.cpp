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
    std::cout<<"sizeof Base:"<<sizeof(Base)
        <<",sizeof Father:"<<sizeof(Father)
        <<",sizeof Mother:"<<sizeof(Mother)
        <<",sizeof Son:"<<sizeof(Son)<<std::endl;
    return 0;
}
