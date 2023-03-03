#include <string>

//引用参数
template <typename T>
inline T const& max (T const& a, T const& b) {
    return a<b ? b:a;
}

template <typename T>
inline T Max (T a, T b) {
    return a<b ? b:a;
}

int main() {
    std::string s;
    ::max("apple","peach");   // OK: same type
    //::max("apple","tomato");//error: no matching
    //::max("apple",s);       //error: no matching


    ::Max("apple","peach"); // OK: same type
    ::Max("apple","tomato");// OK: decayed to same type
    //::Max("apple",s);     // ERROR: deferent type
    return 0;
}
