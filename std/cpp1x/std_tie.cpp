#include <tuple>
#include <iostream>
#include <cassert>

std::tuple<int, double> foo() {
    return std::tuple<int, double>(360, 3.141592654);
}

std::pair<int, double> bar() {
    return std::pair<int, double>(360, 3.141592654);
}

int main() {
    int rad;
    double pi;

    std::tie(rad, pi) = foo();
    assert(rad == 360);
    assert(pi == 3.141592654);

    pi = 0.0;
    std::tie(std::ignore, pi) = foo();
    assert(pi == 3.141592654);

    rad = 0;
    pi = 0.0;
    std::tie(rad, pi) = bar();
    assert(rad == 360);
    assert(pi == 3.141592654);

    //ok: std::tuple<int, double> = std::pair<int, double>
    std::tuple<int, double> t;
    t = bar();

    // error: std::pair can not be assigned by std::tuple
    //std::pair<int, double> p;
    //p = foo();

    std::cout << "std::tie SUCCESS\n";
    return 0;
}
