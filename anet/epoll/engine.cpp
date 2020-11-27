#include "server.h"
#include "engine.h"


int engine_t::start(const char *ip, const int port, server_t* svr) {
    acception_t *ac = new acception_t(_ep, ip, port, svr);
    ac->open();
    _listeners[ac->svrfd()] = ac;
    return 0;
}


int engine_t::run() {
    while (true) {
        _ep->loop();
    }
}
