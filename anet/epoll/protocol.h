#ifndef _NET_PROTOCOL_H_
#define _NET_PROTOCOL_H_

#include "buff.h"

class request_t{
public:
    buff_t *encode(){
        return nullptr;
    }

    int decode(buff_t *buff){
        return 0;
    }
};

class response_t{
public:
    buff_t *encode(){
        return nullptr;
    }

    int decode(buff_t *buff){
        return 0;
    }
};

#endif
