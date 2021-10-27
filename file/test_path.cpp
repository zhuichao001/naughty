#include <stdio.h>
#include "path.h"

int test(){
    bool ok = file_exist("a");
    fprintf(stderr, "file exist:%d\n", ok);

    ok = dir_exist("log/");
    fprintf(stderr, "dir exist:%d\n", ok);


    make_dir("log/");

    ok = dir_exist("log/");
    fprintf(stderr, "after dir exist:%d\n", ok);

    return 0;
}

int main(){
    test();
    return 0;
}
