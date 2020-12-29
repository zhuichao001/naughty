#include <time.h>
#include <sys/time.h>
#include <stdint.h>


enum Precision{SECOND=0, MILISECOND=1,  MICROSECOND=2, NANOSECOND=3};

typedef struct{
    int64_t time;
    int32_t seq;
}timepoint_t;

class watch_t{
public:
    watch_t(Precision p):prec(p){}
    int64_t now(){
        struct timeval tv;
        gettimeofday(&tv,NULL);
        if(prec==SECOND){
            return tv.tv_sec;
        }else if(prec==MILISECOND){
            return tv.tv_sec*1000 + tv.tv_usec/1000;
        }else if(prec==MILISECOND){
            return tv.tv_sec*1000000 + tv.tv_usec;
        }else{
            return 1000*(tv.tv_sec*1000000 + tv.tv_usec);
        }
    }
private:
    Precision prec;
};


class hlc_t{
public:
    hlc_t(Precision p):clock(p){
        least.time = clock.now();
        least.seq = 0;
    }

    clock_t now(){
        int64_t now = clock.now();
        if(now>least.time){
            least.time=now;
            least.seq=0;
        }else{
            least.seq+=1;
        }
    }

    void update(int64_t t){
        if(t>least.time){
            least.time=t;
            least.seq=0;
        }        
        now();
    }
private:
    watch_t clock;
    timepoint_t least;
};
