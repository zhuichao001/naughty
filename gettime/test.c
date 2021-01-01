#include <stdio.h>
#include "clock.h"
#include "tscp.h"


int test_clock(){
    printf("get_time_sec:%ld\n", get_time_sec());
    printf("get_time_nsec:%ld\n", get_time_nsec());
}

int test_tick_ms(){
    printf("get_tick_ms:%ld\n", get_tick_ms());
}

int main(){
    test_clock();
    test_tick_ms();
    return 0;
}
