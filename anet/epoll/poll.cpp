#include <sys/epoll.h>
#include <stdio.h>
#include <errno.h>
#include <memory.h>
#include "io_handler.h"
#include "poll.h"

void epoll_t::update(int op, int fd, int events, void *ptr)const{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = events;
    ev.data.ptr = ptr;
    epoll_ctl(_efd, op, fd, &ev);
}

int epoll_t::loop(){
    struct epoll_event events[1024];
    int n = epoll_wait(_efd, events, 1024, 100); //wait at most 100ms
    if(n==-1){
        if (errno != EINTR) {
            fprintf(stderr, "epoll_wait error.\n");
            return -1;
        }
        //signal_process(base);
        return 0;
    }

    for(int i=0; i<n; ++i){
        io_handler_t *h = (io_handler_t *)(events[i].data.ptr);
        if(events[i].events & EPOLLIN){
            if(h->read()<0){
                h->close();
            };
            h->handle();
        }

        if(events[i].events & EPOLLOUT){
            h->write();
        }

        if ((events[i].events & EPOLLERR) ||
            (events[i].events & (EPOLLRDHUP|EPOLLHUP))) {
            h->close();
        }
    }
}

