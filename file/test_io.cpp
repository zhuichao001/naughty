#include "io.h"

const char * path = "./www.txt";

void test1(){
    int fd = open_create(path);
    printf("open fd: %d\n", fd);
    close(fd);
}

int main() {
    test1();

    append_file(path, "123,./abc\n");

    std::string data;
    read_file(path, data);
    fprintf(stderr, "read:%s\n", data.c_str());

    copy_file(path, "./out.txt");
    return 0;
}
