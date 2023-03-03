#include <iostream>
#include <vector>
#include <stdexcept>

template <typename T>
class Stack{
    std::vector<T> elements;
public:
    void push(T const &t){
        elements.push_back(t);
    }

    T pop(){
        if(elements.size()==0){
            throw std::out_of_range("Stack can not pop when empty.");
        }

        T e = elements.back();
        elements.pop_back();
        return e; 
    }
};

int main(){
    Stack<int> stack;
    //stack.push(6);
    try{
        stack.pop();
    }catch(std::exception const& ex){
        std::cerr << "Catch exception:" << ex.what() << std::endl;
        return -1;
    }
    return 0;
}
