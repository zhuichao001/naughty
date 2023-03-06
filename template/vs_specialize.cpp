
// 类模板
template <class T1, class T2>
class A{
    T1 data1;
    T2 data2;
};

// 函数模板
template <class T>
T max(const T lhs, const T rhs){   
    return lhs > rhs ? lhs : rhs;
}

// 偏特化类模板
template <class T1>
class A<T1,double>{
    T1 data1;
    double data2;
};

// 全特化类模板
template <>
class A<int, double>{
    int data1;
    double data2;
};

// 全特化函数模板
template <>
int max(const int lhs, const int rhs){   
    return lhs > rhs ? lhs : rhs;
}

template <class T>
void f(){ T d; }

// 类型推导
template <>
//void f(){ int d; } // error:error: no function template matches function template specialization 'f'
void f<int>(){ int d; } //OK

int main(){
    return 0;
}
