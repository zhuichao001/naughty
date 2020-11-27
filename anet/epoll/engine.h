#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <assert.h>
#include <map>
#include <set>

#include "poll.h"
#include "acception.h"
#include "server.h"

using namespace std;

const int MAX_CONN_NUMS    = 65536;


class engine_t {
public:
    engine_t(){
        _ep = new epoll_t(1024);
    }
    int start(const char *ip, const int port, server_t* svr);
    int run();
private:
    bool _stat; //running, closing, closed
    map<int, acception_t*> _listeners;
    epoll_t *_ep;
};
