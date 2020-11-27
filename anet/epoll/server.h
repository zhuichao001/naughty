#ifndef _NET_SERVER_H_
#define _NET_SERVER_H_

#include <stdio.h>
#include "protocol.h"

class server_t {
public:
    virtual int process(request_t *req, response_t *res){
        return 0;
    }
    virtual ~server_t(){}
};

class rpc_server_t: public server_t{
public:
    int process(request_t *req, response_t *res){
        printf("rpc server process.\n"); 
        return 0;
    }
};

class raft_server_t: public server_t{
public:
    int process(request_t *req, response_t *res){
        return 0;
    }
};

#endif
