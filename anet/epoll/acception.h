#ifndef _NET_ACCEPTION_H_
#define _NET_ACCEPTION_H_

#include <sys/epoll.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include "io_handler.h"
#include "buff.h"
#include "poll.h"
#include "server.h"

class acception_t: public io_handler_t {
public:
    acception_t(epoll_t *ep, const char *ip, const int port, server_t *svr):_ep(ep), _ip(ip), _port(port), _svr(svr){
    }
    int open();
    int close();
    int read();
    int write();
    int svrfd(){return _svrfd;}
private:
    epoll_t *_ep;
    int _svrfd;
    const char *_ip;
    const int _port;
    server_t *_svr;
};

#endif
