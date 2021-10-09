#include <inttypes.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "depend.h"

#define PAGE_SIZE   (sysconf(_SC_PAGESIZE))
#define PAGE_MASK   (~(PAGE_SIZE - 1))

#define PAGE_START(addr) ((addr) & PAGE_MASK)
#define PAGE_END(addr)   (PAGE_START(addr) + PAGE_SIZE)

void *my_malloc(size_t size){
    printf("%zu bytes memory are allocated by libdepend.so\n", size);
    return malloc(size);
}

int hook(){

    FILE      *fp;
    if((fp = fopen("/proc/self/maps", "r"))==NULL){
        return -1;
    }

    char       line[512];
    uintptr_t  base_addr = 0;
    //find base address of libtest.so
    while(fgets(line, sizeof(line), fp)){
        if(NULL != strstr(line, "libdepend.so") &&
                sscanf(line, "%"PRIxPTR"-%*lx %*4s 00000000", &base_addr) == 1)
            break;
    }
    fclose(fp);

    if(0 == base_addr){ 
        return -1;
    }
    //the absolute address
    uintptr_t addr = base_addr + 0x201028;

    //add write permission
    mprotect((void *)PAGE_START(addr), PAGE_SIZE, PROT_READ | PROT_WRITE);

    //replace the function address
    *(void **)addr = my_malloc;

    //clear instruction cache
    __builtin___clear_cache((void *)PAGE_START(addr), (void *)PAGE_END(addr));
    return 0;
}

int main(){
    int err = hook(); 
    if(err<0){
        fprintf(stderr, "hook failed\n");
        return -1;
    }

    display();

    return 0;
}
