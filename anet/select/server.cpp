#include "server.h"
#include "util.h"


int server_engine_t::_init_socket() {
    svrfd = socket(AF_INET, SOCK_STREAM,0);
    if (svrfd == -1) {
        fprintf(stderr, "create socket fail,erron:%d,reason:%s\n", errno, strerror(errno));
        return -1;
    }

    set_unblocking(svrfd, 1);

    set_reuseaddr(svrfd, 1);
    bind_port(svrfd, ip, port);
    listen(svrfd, 128);
    
    fprintf(stdout, "init server fd:%d\n", svrfd);
    return svrfd;
}


int server_engine_t::start() {
    int fd = _init_socket();
    fs.add_r(fd);

    running = true;
    while (running) {
        _loop();
    }
}


int server_engine_t::_loop() {
    vector<int> ready_r, ready_w;
    _get_ready(ready_r, ready_w);
    for(vector<int>::iterator it=ready_r.begin(); it!=ready_r.end(); ++it) {
		int fd = *it;
        if(fd!=svrfd) {
            iohandler_t *handler = _get_handler(fd);
            if(handler==NULL){
                fprintf(stderr, "fail is null, fd:%d\n", fd);
            } else {
                fprintf(stderr, "call readfrom, fd:%d\n", fd);
                ioevent_t *ev = handler->readfrom();
                
                fprintf(stderr, "read event, type:%d\n", ev->type);
                if (ev!=NULL) {
                    _on_event(ev, handler);
                }
            }
        } else {
             fprintf(stderr, "call _accept for svrfd:%d\n", fd);
             _accept();
        }
    }

    for(vector<int>::iterator it=ready_w.begin(); it!=ready_w.end(); ++it) {
		int fd = *it;
        fprintf(stdout, "write for fd:%d\n", fd);
        iohandler_t *handler = _get_handler(fd);
        if(handler==NULL){
            fprintf(stderr, "writing get handler fail for fd:%d\n", fd);
            continue;
        }
        ioevent_t *ev = handler->writeto();
        fprintf(stderr, "write event, type:%d\n", ev->type);
        if (ev!=NULL) {
            _on_event(ev, handler);
        }
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


int server_engine_t::_add_handler(int fd) {
    if(handlers.find(fd)==handlers.end()){
        handlers[fd] = new iohandler_t(fd);
        return 1;
    }
    return 0;
}


int server_engine_t::_del_handler(int fd) {
    if(handlers.find(fd)==handlers.end()){
        return -1;
    }
    delete handlers[fd];
    return 0;
}


int server_engine_t::_get_ready(vector<int> &ready_r, vector<int> &ready_w) {
    struct timeval tv;
    tv.tv_sec=2;
    tv.tv_usec=0;

    memcpy(&fs._r, &fs.r, sizeof(fd_set));
    memcpy(&fs._w, &fs.w, sizeof(fd_set));

    int ret = select(fs.get_maxfd()+1, &fs._r, &fs._w, NULL, &tv);
    if (ret == 0) { //TIME_OUT
        fprintf(stderr, "select timeout.\n");
        return 0;
    } else if (ret == -1) {
        fprintf(stderr, "select error:%s.\n", strerror(errno));
        return -1;
    }

    for (set<int>::iterator it=fs.allfds.begin(); it!=fs.allfds.end(); ++it) {
        int fd = *it;
        if (fs.readable(fd)) {
            fprintf(stderr, "isset read, fd:%d\n", fd);
			ready_r.push_back(fd);
        }
        if (fs.writeable(fd)) {
            fprintf(stderr, "isset wriete, fd:%d\n", fd);
			ready_w.push_back(fd);
        }
    }

    return 0;
}


int server_engine_t::_on_event(ioevent_t *ev, iohandler_t *h) {
    if ((ev->type & EV_CONN) != 0) {
        fprintf(stdout, "EV_CONN, fd:%d | type:%d & %d = %d \n", ev->fd, ev->type, EV_CONN, (ev->type&EV_CONN));
        fs.add_r(ev->fd);
        _add_handler(ev->fd); 
    } else if ((ev->type & EV_DISCONN) != 0) {
        fprintf(stdout, "EV_DISCONN, fd:%d | type:%d & %d = %d \n", ev->fd, ev->type, EV_DISCONN, (ev->type&EV_DISCONN));
        fs.del(ev->fd);
        _del_handler(ev->fd); 
    } 

    if ((ev->type & EV_DATA_IN) != 0) {
        fprintf(stdout, "EV_DATA_IN, fd:%d, buff: %s | type:%d & %d = %d \n", ev->fd, h->rbuff.getdata(), ev->type, EV_DATA_IN, (ev->type&EV_DATA_IN));
    } 

    if ((ev->type & EV_DATA_OUT) != 0) {
        fprintf(stdout, "EV_DATA_OUT, fd:%d | type:%d & %d = %d  \n", ev->fd, ev->type, EV_DATA_OUT, (ev->type & EV_DATA_OUT));
    }
    return 0;
}
