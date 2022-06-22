#include <vector>
#include <iostream>
#include <string>
#include <utility>

//purpose: avoid copy
void std_move_case() {
    std::string str = "Hello, world.";
    std::vector<std::string> v;
 
    v.push_back(str);
    std::cout << "After copy, str is \"" << str << "\"\n";
 
    v.push_back(std::move(str));
    std::cout << "After move, str is \"" << str << "\"\n";
 
    std::cout << "The contents of the vector are \"" << v[0] << "\", \"" << v[1] << "\"\n";
}

//how to understand
void std_move_is(){
    {
        int &&ref_a = 5;
        ref_a = 6; 
    }
     
    //A.K.A
    {
        int temp = 5;
        int &&ref_a = std::move(temp);
        ref_a = 6;
    }
}

void std_move_self(){
    auto change = [](int& left_value) {
        left_value = 8;
    };

    int a = 5;
    //a_right self is left-value, although std::move(a) is right-value
    int &&a_right = std::move(a); 
    change(a_right);
    fprintf(stderr, "a:%d\n", a);
}

int main() {

    std_move_case();
    return 0;
}
