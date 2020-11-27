
#include <unistd.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include "protocol.h"
#include "buff.h"
#include "connection.h"


int connection_t::open(){
    _ep->update(EPOLL_CTL_ADD, _fd, EPOLLIN|EPOLLOUT, (void*)this);
    return 0;
}

int connection_t::close(){
    _ep->update(EPOLL_CTL_DEL, _fd, 0, nullptr);
    ::close(_fd);
    return 0;
}

int connection_t::read(){
    while(true){
        char *data = nullptr;
        int len = 0;
        _rb.get(&data, &len);

        int n = ::read(_fd, data, len);
        if(n<0 && errno==EAGAIN){// read done
            break;
        }else if(n==0){ //conn closed
            ::close(_fd);
            return -1;
        }else if(n<0){ //read error
            ::close(_fd);
            fprintf(stderr,"client: read errno:%d.\n", errno);
            return -1;
        }else if(n<len){ //normal
            break;
            fprintf(stdout,"client fd=%d read.\n", _fd);
        }
    }
    return 0;
}

int connection_t::write(){
    while(!_wb.empty()){
        char *data = nullptr;
        int len = 0;
        _wb.get(&data, &len); 

        int n = ::write(_fd, (void *)data, (size_t)len);
        if (n<0 && errno == EAGAIN) { //tcp buffer is full
            break;
        } else if (n<=0) { //error
            this->close();
            _wb.reset();
            fprintf(stderr,"client: write errno:%d.\n", errno);
            break;
        } else {
            _wb.clear(n);
        }
    }
    return 0;
}

int connection_t::handle(){
    request_t req;
    int err = req.decode(&_rb);
    if(err<0){
        return -1;
    }else if(err==0){
        return 0;
    }

    response_t res;
    int result = _svr->process(&req, &res);
    res.decode(&_wb);
    this->write();
    return 0;
}
