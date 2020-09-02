#include "cow_string.h"
#include <stdio.h>



void test(){
    cow_string s("abdddddddddddddddddddddddddc");
    cow_string t(s);
    printf("%s\n", s.c_str());
    printf("%s\n", t.c_str());
}


int main(){
    test();
    return 0;
}
