#include "eager_string.h"
#include <stdio.h>



void test(){
    eager_string s("abdddddddddddddddddddddddddc");
    eager_string t(s);
    printf("%x, %s\n", s.c_str(), s.c_str());
    printf("%x, %s\n", t.c_str(), t.c_str());

    t[0] = 'A';
    printf("%x, %s\n", s.c_str(), s.c_str());
    printf("%x, %s\n", t.c_str(), t.c_str());
}


int main(){
    test();
    return 0;
}
