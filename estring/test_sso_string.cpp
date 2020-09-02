#include "sso_string.h"
#include <stdio.h>



void test(){
    sso_string s("abdddddddddddddddddddddddddc");
    printf("%s\n", s.c_str());
}


int main(){
    test();
    return 0;
}
