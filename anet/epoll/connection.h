#ifndef _NET_CONNECTION_H_
#define _NET_CONNECTION_H_

#include "io_handler.h"
#include "poll.h"
#include "server.h"
#include "buff.h"


class connection_t: public io_handler_t {
public:
    connection_t(epoll_t *ep, int fd, server_t *svr):_ep(ep), _fd(fd), _svr(svr), _rb(2048), _wb(4096){
    }
    int open();
    int close();
    int read();
    int write();
    int handle();
private:
    int _efd;
    int _fd;
    buff_t _rb;
    buff_t _wb;
    server_t *_svr;
    epoll_t *_ep;
};

#endif
