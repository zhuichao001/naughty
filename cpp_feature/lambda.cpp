#include <vector>
#include <algorithm>
#include <stdio.h>

//[ capture list ] ( parameter list) -> return type { function body }


void test1(){
    auto plus = [] (int v1, int v2) -> int { return v1 + v2; };
    int sum = plus(1, 2);
    printf("sum:%d\n", sum);
}

void test2(){
    struct Item {
        int k;
        int v;
    };

    std::vector<Item> vec = {{3,7},{1,19},{6,9},{2,5}};

    std::sort(vec.begin(), vec.end(),
            [] (const Item& e1, const Item& e2) { return e1.k < e2.k; });

    std::for_each(vec.begin(), vec.end(),
            [] (const Item& e) { printf("%d:%d\n", e.k, e.v); });
}

int main(){
    //test1();
    test2();
    return 0;
}
