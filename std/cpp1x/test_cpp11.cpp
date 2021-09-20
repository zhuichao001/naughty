#include <iostream>
#include <map>
#include <vector>
#include <tuple>

using namespace std;

void case_auto(){
    auto x = 3;  
    decltype(x) y = x; 
}

void case_tuple(){
    auto t = make_tuple(1, 2.0, 'w', "C++11");
    printf("Value of t is (%d,%.2f,%c,%s)\n", get<0>(t), get<1>(t), get<2>(t), get<3>(t));
}

void case_init(){
    int arr[3]{1, 2, 3};
    vector<int> vec{1, 2, 3};
    map<int, string> dict{{1, "a"}, {2, "b"}};
    string str{"handsome case."};
}

void case_for_arr(){
    int a[5] = { 1, 2, 2, 5, 1 };
    for( int i : a ) {
        printf( "%d : %d\n", i, a[i] );
    }
}

void case_for_map(){
    map<string, int> m{{"a", 1}, {"b", 2}, {"c", 3}};  
    for( auto p : m ) {  
        cout<<p.first<<" : "<<p.second<<endl;  
    } 
}

void case_for_each(){
    case_for_map();
    case_for_arr();
}

void case_lambda(){
    auto lambda = [](int x, int y) {return x + y;};
    cout<<"sum:"<<lambda(2,3)<<endl;
}

void case_nullptr(){
    struct M{
        void Do(int i){cout<<"Do(int):"<<i<<endl;} 
        void Do(int *i){cout<<"Do(intptr):"<<i<<endl;} 
    };
    struct M m; 
    m.Do(nullptr);
}

void case_default_delete(){
    struct M{
        M() = default; 
        ~M() = default; 
        M(const M&) = delete;
        M& operator=(const M&) = delete;
        void *operator new(size_t) = delete;
        void *operator new[](size_t) = delete;
    };

    M m;
    // wrong usage
    // M n(m);
    // M x = m;
    // M y = new M;
    // M z = new M[3]
}

int main(){
    case_auto();
    case_for_each();
    case_init();
    case_tuple();
    case_lambda();
    case_nullptr();
    case_default_delete();
}
