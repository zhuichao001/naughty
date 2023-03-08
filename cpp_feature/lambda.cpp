#include <stdio.h>

//[ capture list ] ( parameter list) -> return type { function body }


void test(){
    auto plus = [] (int v1, int v2) -> int { return v1 + v2; };
    int sum = plus(1, 2);
    printf("sum:%d\n", sum);
}

int main(){
    test();
    return 0;
}
