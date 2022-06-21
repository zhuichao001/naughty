#include <stdio.h>
#include <sys/select.h>
#include <thread>

void sleep_ms(unsigned int ms){
    struct timeval tval;
    tval.tv_sec = ms/1000;
    tval.tv_usec = (ms*1000)%1000000;
    select(0, NULL, NULL, NULL, &tval);
}

void thread_sleep_ms(unsigned int ms){
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main(){
    fprintf(stderr, "sleep begin\n");
    thread_sleep_ms(1000);
    fprintf(stderr, "sleep end\n");
    return 0;
}
