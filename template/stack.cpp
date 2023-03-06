#include <iostream>
#include <vector>
#include <stdexcept>

template <typename T>
class Stack{
    std::vector<T> elements;
public:
    bool empty() const {
        return elements.empty();
    }

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

    T top(){
        if(elements.size()==0){
            throw std::out_of_range("Stack is empty.");
        }

        return elements.front();
    }
};

int main(){
    Stack<int> stack;
}
