#include <iostream>
#include <vector>
#include <string>

class SomeType {
public:
    SomeType(int id, const std::string &name):
        id_(id),
        name_(name) { 
            std::cout << "normal construct SomeType." << name_ << std::endl;
    }

    SomeType(const SomeType &t):
        id_(t.id_),
        name_(t.name_) {  
            std::cout << "copy construct SomeType:" << name_ << std::endl;
    }

    SomeType(const SomeType &&t) {
        id_ = std::move(t.id_);
        name_ = std::move(t.name_);
            std::cout << "move contruct SomeType:" << name_ << std::endl;
    }

    ~SomeType() {
        std::cout << "destory SomeType SomeType:" << name_ << std::endl;
    }
private:
    int id_;
    std::string name_;
};

int main(int argc, char *argv[]) {
    std::vector<SomeType> vec;
    vec.reserve(2);

    std::cout << "\n ------ push_back --------" << std::endl;
    vec.push_back(SomeType(1, "Lucy"));

    std::cout << "\n ------ emplace_back --------" << std::endl;
    vec.emplace_back(2, "Kitty");

    std::cout << "\n -------- finish -------- " << std::endl;
}
