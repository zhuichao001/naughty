#include <stdio.h>
#include "ufifo.h"
#include <thread>


ufifo u(117);
const int N = 1000000*26;

void write(){
    char tmp[] = "abcdefghijklmnopqrstuvwxyz";
    char src[N+1];
    for(int i=0; i<N/26; ++i){
        memcpy(src+i*26, tmp, 26);
    }
    src[N]=0;

    for(int pos=0; pos<N; ){
        int len = u.push((uint8_t*)src+pos, 26);
        if(len==0)continue;

        char back = src[pos+len];
        src[pos+len] = 0;
        printf("write len=%d, data=%s\n", len, src+pos);
        src[pos+len] = back;

        pos+=len;
    }
}

void read(){
    char dst[N+1];
    memset(dst, 0, N+1);
    for(int pos=0; pos<N; ){
        int len = u.pop((uint8_t*)dst+pos, 26);
        if(len==0)continue;

        printf("read len=%d, data=%s\n", len, dst+pos);

        pos+=len;
    }
}

int main(){
    std::thread thr(read);
    std::thread thw(write);
    thr.join();
    thw.join();
    printf("finished\n");
}
