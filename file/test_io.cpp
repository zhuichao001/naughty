#include "io.h"

const char * path = "./www.txt";

void test1(){
    int fd = open_create(path);
    printf("open fd: %d\n", fd);
    close(fd);
}

void test2(){
    append_file(path, "123,./abc\n");

    std::string data;
    read_file(path, data);
    fprintf(stderr, "read:%s\n", data.c_str());
}

void test3(){
    copy_file(path, "./out.txt");
}

int main() {
    test1();
    test2();
    test3();
    return 0;
}
