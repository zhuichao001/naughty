#include "fio.h"

const char * path = "./www.txt";

void test1(){
    int fd = fio::fopen(path, fio::READ);
    printf("open fd: %d\n", fd);
    fio::fclose(fd);
}

void test2(){
    int fd = fio::fopen(path, fio::APPEND);
    fio::fappend(fd, "123,./abc\n");
    fio::fclose(fd);

    fd = fio::fopen(path, fio::READ);
    std::string data;
    fio::fread(fd, data);
    fprintf(stderr, "read:%s\n", data.c_str());
    fio::fclose(fd);
}

void test3(){
    fio::fcopy(path, "./out.txt");
}

void test4(){
    int total = fio::dircap("/root/workspace/src/github.com/zhuichao001/naughty/fio/");
    printf("total:%d\n", total);
}

void test5(){
    fio::mkdir("/tmp/a/b/c");
}

int main() {
    //test1();
    //test2();
    //test3();
    //test4();
    test5();
    return 0;
}
