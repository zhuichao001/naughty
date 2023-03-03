#include <string>
#include <deque>
#include "stack.hpp"

template<>
class Stack<std::string>{
    std::deque<std::string> elements;
public:
    void push(std::string const &t);
    std::string pop();
};

void Stack<std::string>::push(std::string const &t){
    elements.push_back(t);
}

std::string Stack<std::string>::pop(){ //no throw exceptions
    std::string e = elements.back();
    elements.pop_back();
    return e; 
}

int main(){
    Stack<std::string> sstack;
    sstack.push("abc");
    auto s = sstack.pop();
    std::cout << "pop " << s << std::endl;
    return 0;
}
