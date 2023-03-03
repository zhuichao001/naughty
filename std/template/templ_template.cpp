#include <deque>

template <typename T, template <typename> class CONTAI = std::deque >
class Stack {
private:
    CONTAI<T> elements;
public:
    void push(T const&);
    void pop();
    T top() const;
    bool empty() const { return elements.empty(); }
};

template <typename T, template <typename> class CONTAI>
void Stack<T,CONTAI>::push (T const& e) {
    elements.push_back(e);
}

int main(){
    return 0;
}
