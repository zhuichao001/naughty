#include "server.h"
#include "util.h"


int server_engine_t::_build() {
    int sfd = socket(AF_INET, SOCK_STREAM,0);
    if (sfd == -1) {
        fprintf(stderr, "create socket fail,erron:%d,reason:%s\n", errno, strerror(errno));
        return -1;
    }

    set_reuseaddr(sfd, 1);
    bind_port(sfd, ip, port);
    listen(sfd, 128);

    FD_SET(sfd, &fdset);
    fds.insert(sfd);
    handlers[sfd] = new siohandler_t(sfd);

    return sfd;
}


int server_engine_t::start() {
    running = true;
    _build();
    while (running) {
        _loop();
    }
}


int server_engine_t::_set_prepare(struct timeval *tvp) {
    tvp->tv_sec=2; 
    tvp->tv_usec=0;

    for (set<int>::iterator it=fds.begin(); it!=fds.end(); ++it) {
        FD_SET(*it, &fdset);
        maxfd = max(maxfd, *it);
    }

}

int server_engine_t::_loop() {
    struct timeval tv;
    _set_prepare(&tv);

    int ready_fds[fds.size()];
    int ready_num = 0;

    _get_ready(ready_fds, &ready_num, &tv);

    for(int i=0; i<ready_num; ++i) {
        iohandler_t *handler = _get_handler(ready_fds[i]);
        if(handler==NULL){
            fprintf(stderr, "get handler fail for fd:%d\n", ready_fds[i]);
            continue;
        }
        ioevent_t *ev = handler->readfrom();
        _on_event(ev);
    }
}
 
iohandler_t* server_engine_t::_get_handler(int fd) {
    hash_map<int, iohandler_t*>::iterator it = handlers.find(fd);
    if(it==handlers.end()){
        return NULL;
    } else {
        return it->second;
    }
}

int server_engine_t::_del_handler(int fd) {
    hash_map<int, iohandler_t*>::iterator it = handlers.find(fd);
    if(it==handlers.end()){
        return -1;
    } else {
        delete it->second;
        return 0;
    }
}

int server_engine_t::_get_ready(int *ready_fds, int *ready_n, struct timeval *tvp) {
    if (ready_fds == NULL || ready_n == NULL || tvp==NULL) {
        return -1;
    }

    int ret = select(maxfd+1, &fdset, NULL, NULL, tvp);
    if (ret == 0) {
        fprintf(stdout, "select is timeout.\n");
        return 0;
    } else if (ret == -1) {
        fprintf(stderr, "select error:%s.\n", strerror(errno));
        return errno;
    }

    *ready_n = 0;
    for (set<int>::iterator it = fds.begin(); it!=fds.end(); ++it) {
        int fd = *it;
        if (FD_ISSET(fd, &fdset)) {
           ready_fds[*ready_n] = fd;
           *ready_n += 1;
        }
    }
    return *ready_n;
}

int server_engine_t::_on_event(ioevent_t *ev) {
    if (ev->type==EV_CONN) {
        FD_SET(ev->fd, &fdset);
        fds.insert(ev->fd);
        handlers[ev->fd] = new ciohandler_t(ev->fd);
    } else if (ev->type==EV_DISCONN) {
        close(ev->fd);
        FD_CLR(ev->fd, &fdset);
        fds.erase(ev->fd);
        _del_handler(ev->fd); 
    } else if (ev->type==EV_DATA_IN) {
        fprintf(stdout, "IN: %s\n", ev->buff);
        //TODO  put data=> session
    } else if (ev->type==EV_DATA_OUT) {
    }
    return 0;
}


int main(int argc,char *argv[]) {
    server_engine_t svr("127.0.0.1", 9033);
    svr.start();
    return 0;
}
