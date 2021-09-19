#ifndef _TIME_HLC_WATCH_H_
#define _TIME_HLC_WATCH_H_

#include <time.h>
#include <sys/time.h>


enum class Precision{
    SECOND=0, 
    MILISECOND=1,  
    MICROSECOND=2, 
    NANOSECOND=3
};


class watch_t{
public:
    watch_t(Precision p):prec(p){}
    int64_t now(){
        struct timeval tv;
        gettimeofday(&tv,NULL);
        if(prec==Precision::SECOND){
            return tv.tv_sec;
        }else if(prec==Precision::MILISECOND){
            return tv.tv_sec*1000 + tv.tv_usec/1000;
        }else if(prec==Precision::MILISECOND){
            return tv.tv_sec*1000000 + tv.tv_usec;
        }else{
            return 1000*(tv.tv_sec*1000000 + tv.tv_usec);
        }
    }
private:
    Precision prec;
};

#endif
