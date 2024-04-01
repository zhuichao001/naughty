#include <functional>
#include <iostream>

using namespace std;

int normal(int k) {
    return -k;
}

auto lambda = [](int k)->int{ return k*k; };

class functor {
  public:
    int operator()(int k) {
        return k/2;
    }
};

class counter {
public:
    int inc(int k) { return k+1; }
    int dec(int k) { return k-1; }
    static int digits(int k) { int n=0; while(k!=0){k/=10;++n;}; return n; }
};

int main() {
    std::function<int(int)> mathcall;

    mathcall = normal;
    cout << "normal:" << mathcall(1) << endl;

    mathcall = lambda;
    cout << "lambda:" << mathcall(2) << endl;

    functor f;
    mathcall = f;
    cout << "functor:" << mathcall(3) << endl;

    counter caller;
    mathcall = std::bind(&counter::inc, caller, std::placeholders::_1);
    cout << "class member func:" << mathcall(4) << endl;

    mathcall = counter::digits;
    cout << "class static func:" << mathcall(15) << endl;

    return 0;
}
