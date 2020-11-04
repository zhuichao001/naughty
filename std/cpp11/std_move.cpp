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


int main(){
    std_move_case();
    return 0;
}
