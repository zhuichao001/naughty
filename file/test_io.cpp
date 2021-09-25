#include "io.h"

const char * path = "./www.txt";

void test1(){
    int fd = open_create(path);
    printf("open fd: %d\n", fd);
    close(fd);
}

void test2(){
    int fd = open_append(path);
    append_file(fd, "123,./abc\n");
    close(fd);

    fd = open_read(path);
    std::string data;
    read_file(fd, data);
    fprintf(stderr, "read:%s\n", data.c_str());
    close(fd);
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
