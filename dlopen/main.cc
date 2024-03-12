#include <dlfcn.h>
#include <stdio.h>

typedef int(*function)();

void call_by_so(){
    void *handle=dlopen("/root/workspace/src/github.com/zhuichao001/naughty/dlopen/libfoo.so", RTLD_LAZY);
    if(handle==nullptr) {
        printf("dlopen failed %s\n", dlerror());//打印错误信息
        return;
    }

    function fun = (function)dlsym(handle,"bar");
    if (fun == nullptr) {
        printf("ERROR:%s:dlsym\n", dlerror());
        return;
    }

    fun();

    dlclose(handle);
}

int main(){
    call_by_so();
    return 0;
}
