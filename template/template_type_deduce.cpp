#include <string>

template<class T1, class T2>
class SomeType{
    int id_;
    T1 t1_;
    T2 t2_;
public:
    SomeType(const int id, const T1 &t1, const T2 &t2):
        id_(id),
        t1_(t1),
        t2_(t2){
    }
};


int main(){
    SomeType s(3, true, 1.236);

    SomeType<double, std::string> t(3, 3.12, "ok");
    return 0;
}
