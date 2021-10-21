#include "mmap_file.h"


int checkrecord(memfile &m, int index){
    char *data;
    int len;
    int err = m.read(index, &data, &len);
    if(err == 0){
        fprintf(stderr, "read index %d: %s\n", index, data);
    }else{
        fprintf(stderr, "read index %d failed\n", index);
    }
    return 0;
}

int main(){
    {
        memfile m; 
        m.create("./a.txt");
        m.write(1, "001\0", 4);
        m.write(2, "002\0", 4);
        m.write(3, "003\0", 4);

        m.rename("./a.txt", "b.txt");

        m.write(4, "004", 4);
    }

    {
        memfile m;
        m.load("./b.txt");
        checkrecord(m, 3);
        checkrecord(m, 4);
        checkrecord(m, 5);
    }
    return 0;
}
