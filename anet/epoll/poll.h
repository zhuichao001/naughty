#ifndef _NET_EPOLL_H_
#define _NET_EPOLL_H_

#include <sys/epoll.h>
#include <errno.h>
#include <memory.h>

class epoll_t {
public:
    epoll_t(const int esize){
        _efd = epoll_create(esize);
    }

    void update(int op, int fd, int events, void *ptr)const;
    int loop();
private:
    int _efd;
    //std:hash_map<int, connection_t*> _hub;
};

#endif
