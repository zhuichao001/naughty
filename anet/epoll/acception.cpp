
#include "acception.h"
#include "connection.h"
#include "socket.h"
#include "poll.h"

int acception_t::open(){
    _svrfd = server_socket(_ip, _port);
    assert(_svrfd>0);
    _ep->update(EPOLL_CTL_ADD, _svrfd, EPOLLIN | EPOLLET, this);
    return 0;
}

int acception_t::close() {
    exit(-1);
}

int acception_t::read() {
    int fd = accept(_svrfd);
    if(fd<0){
        return -1;
    }
    connection_t *h = new connection_t(_ep, fd, _svr);
    h->open();
    return 0;
}

int acception_t::write() {
    return 0;
}
