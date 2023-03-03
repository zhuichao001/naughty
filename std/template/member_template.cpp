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

    template<typename T2>
    Stack<T> & operator=(const Stack<T2> &);
};

template<typename T>
template<typename T2>
Stack<T> & Stack<T>::operator=(const Stack<T2> &src){
    if ((void*)this == (void*)&src) {
        return *this;
    }
    Stack<T2> tmp(src);
    elements.clear();
    while (!tmp.empty()) {
        elements.push_back(static_cast<T>(tmp.top()));
        tmp.pop();
    }
    return *this;
}

int main(){
    Stack<double> dstack;
    Stack<int> istack;
    dstack = istack;
    return 0;
}
