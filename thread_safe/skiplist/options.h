
#include "snapshot.h"

//for open db
typedef struct options{
    int gc_interval_us;
}opt_t;

///for read
typedef struct roptions{
    const snapshot *snap;
    roptions(): snap(nullptr){
    }
}ropt_t;

//for write
struct woptions{
    bool flush;
}wopt_t;

#endif
