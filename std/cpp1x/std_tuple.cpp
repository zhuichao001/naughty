#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>

std::tuple<double, char, std::string> get_student(int id) {
    if (id == 0) return std::make_tuple(3.8, 'A', "John");
    if (id == 1) return std::make_tuple(3.1, 'B', "Smith");
    if (id == 2) return std::make_tuple(2.6, 'C', "William");
    throw std::invalid_argument("id");
}

void case1(){
    auto student = get_student(0);
    std::cout       << "ID: 0, "
        << "GPA: "  << std::get<0>(student) << ", "
        << "grade: "<< std::get<1>(student) << ", "
        << "name: " << std::get<2>(student) << '\n';
}

void case2(){
    double gpa;
    char grade;
    std::string name;
    std::tie(gpa, grade, name) = get_student(1);
    std::cout       << "ID: 1, "
        << "GPA: "  << gpa << ", "
        << "grade: "<< grade << ", "
        << "name: " << name << '\n';
}

void case3(){
    //c++17 feature:
    auto [ gpa, grade, name ] = get_student(2);
    std::cout       << "ID: 2, "
        << "GPA: "  << gpa << ", "
        << "grade: "<< grade << ", "
        << "name: " << name << '\n';
}

int main() {
    case1();
    case2();
    case3();
}
