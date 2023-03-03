#include <deque>

template <typename T, template <typename ELEM> class CONTAI = std::deque >
class Stack {
private:
    CONTAI<T> elements;
public:
    void push(T const&);
    void pop();
    T top() const;
    bool empty() const { return elements.empty(); }
};

int main(){
    return 0;
}
