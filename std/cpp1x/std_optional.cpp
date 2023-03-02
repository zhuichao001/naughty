#include <iostream>
#include <optional>

struct Out {
    std::string name { "" };
    int id { 0 };
};

std::optional<Out> func(const std::string& in) {
    Out o;
    if (in.size() == 0)
        return std::nullopt;
    o.name = "hello";
    o.id = 101;
    return { o };
}

int main() {
    if (auto ret = func("hi"); ret.has_value()) {
        std::cout << ret->name << std::endl;
        std::cout << ret->id << std::endl;
    }
    return 0;
}
