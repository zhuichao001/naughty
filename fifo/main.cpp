#include <stdio.h>
#include "ufifo.h"
#include <thread>


ufifo u(117);
const int N = 10000*26;

void write(){
    char tmp[] = "abcdefghijklmnopqrstuvwxyz";
    char src[N+1];
    for(int i=0; i<N/26; ++i){
        memcpy(src+i*26, tmp, 26);
    }
    src[N]=0;

    for(int pos=0; pos<N; ){
        int len = u.push((uint8_t*)src+pos, N-pos);
        if(len==0)continue;

        { //print what's pushed
            char backup = src[pos+len];
            src[pos+len] = 0;
            fprintf(stderr, "write len=%d, data=%s\n", len, src+pos);
            src[pos+len] = backup;
        }

        pos+=len;
    }
}

void read(){
    char dst[N+1];
    for(int pos=0; pos<N; ){
        int len = u.pop((uint8_t*)dst+pos, 26); //read 26 at max
        if(len==0)continue;
        dst[pos+len] = 0;

        fprintf(stderr, "read len=%d, data=%s\n", len, dst+pos);

        pos+=len;
    }

}

int main(){
    std::thread thr(read);
    std::thread thw(write);
    thw.join();
    thr.join();
    printf("finished\n");
}
