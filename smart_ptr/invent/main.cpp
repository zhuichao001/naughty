#include <stdio.h>
#include "smart_ptr.h"


int main(){
    shared_ptr<int> m;
    {
        shared_ptr<int> p(new int);
        *p = 123;
        printf("val:%d, use_count:%d\n", *p, p.use_count());

        m = p;
        printf("val:%d, use_count:%d\n", *m, m.use_count());
    }
    printf("val:%d, use_count:%d\n", *m, m.use_count());

    return 0;
}
