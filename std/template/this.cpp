template <typename T>
class Base {
public:
    void Exit(){}
};

template <typename T>
class Derived : Base<T> {
public:
    void Foo() {
        //Exit();//Error: 调用外部的exit()或者出现错误
        this->Exit(); //OK
        Base<T>::Exit(); //OK
    }
};

int main(){
    Derived<int> der;
    der.Foo();
    return 0;
}
