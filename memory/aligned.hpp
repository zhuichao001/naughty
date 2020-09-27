#include <stdio.h>
#include <stdint.h>

struct unpacked_struct {
    char _c;
    int _i;
};
         
struct packed_struct {
    char _c;
    int _i;
    struct unpacked_struct _s;
}__attribute__ ((__packed__));

struct aligned_struct {
    char _s5[5];
    uint32_t _i32;
    char _s3[3];
} __attribute__((aligned(4))); 


void test(){
    printf("sizeof unpacked_struct:%d\n", sizeof(struct unpacked_struct));
    printf("sizeof packed_struct:%d\n", sizeof(struct packed_struct));
    printf("sizeof aligned_struct:%d\n", sizeof(struct aligned_struct));
}

int main(){
    test();
    return 0;
}
