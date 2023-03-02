#include <iostream>
#include <string>
#include <string_view>

int main() {
    const char* cstr = "hello world.";
    std::string_view strv1(cstr);
    std::string_view strv2(cstr, 5);
    std::cout << "view1: " << strv1 << ", view2: " << strv2 << std::endl;

    std::string str = "the same world";
    std::string_view strv3(str.c_str());
    std::string_view strv4(str.c_str(), 3);
    std::cout << "view3: " << strv3 << ", view4: " << strv4 << std::endl;
    return 0;
}
