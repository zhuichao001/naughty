#include <vector>
#include <algorithm>
#include <execution>

int main()
{
    std::vector<int> coll;
    coll.reserve(10000);
    for (int i = 0; i < 10000; ++i) {
        coll.push_back(i);
    }

    std::for_each(std::execution::par, coll.begin(), coll.end(), [](auto& val) {
        val *= val;
    });
}
