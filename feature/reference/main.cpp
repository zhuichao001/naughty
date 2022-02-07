#include <utility>
#include <stdio.h>

void foo(int && n) {
        n += 1; // ok
}

void test1(){
    /*
     * int &&ref_a = 5;
     * ref_a = 6; 
     * */
    int temp = 5;
    int &&ref_a = std::move(temp);
    ref_a = 6;
    printf("temp:%d, ref_a:%d\n", temp, ref_a);
}

void test2(){
    int m = 3;
    foo(std::move(m));
    printf("m:%d\n", m);

}

int main(){
    test1();
    return 0;
}
