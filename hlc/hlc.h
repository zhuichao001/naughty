#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#include "watch.h"


typedef struct{
    int64_t time;
    int32_t seq;
}vtime_t;


class hlc_t{
public:
    hlc_t(Precision p):clock(p){
        least.time = clock.now();
        least.seq = 0;
    }

    vtime_t now(){
        int64_t now = clock.now();
        if(now>least.time){
            least.time=now;
            least.seq=0;
        }else{
            least.seq+=1;
        }
        return least;
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
    vtime_t least;
};
