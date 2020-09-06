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

#include <hash_map>
#include <set>

using namespace __gnu_cxx;
using namespace std;

const int MAX_CONN_NUMS	= 1024;

enum evtype_t {
	EV_DATA_IN = 1,
    EV_DATA_OUT = 2, 
    EV_CONN = 3,
    EV_DISCONN = 4,
};

class ioevent_t {
  public:
    ioevent_t(int _fd, evtype_t _t, int _len = 0){
        fd = _fd;
        type = _t;
        if(_len > 0){
            len = _len;
            buff = new char[len];
        } else {
            len = 0;
            buff = NULL;
        }
    }
    ~ioevent_t(){
        if(buff) {
            delete buff;
            buff = NULL;
        }
    }
    evtype_t type;
    int fd;
    char *buff;
    int len;
};

class iohandler_t {
  public:
	virtual ioevent_t* readfrom() {return NULL;};
	virtual int writeto() {return 0;};
};

class ciohandler_t : public iohandler_t {
  public:
    ciohandler_t(int d):_fd(d){}
	virtual ioevent_t* readfrom() {
        const int LINE_SIZE = 512;
        ioevent_t *e = new ioevent_t(_fd, EV_DATA_IN, LINE_SIZE);
        int n = read(_fd, e->buff, LINE_SIZE);
        if (n <= 0) {
            delete e;
            fprintf(stderr,"client: server is closed.\n");
			e = new ioevent_t(_fd, EV_DISCONN);
        }
        return e;
    }
	virtual int writeto() {}
  private:
	int _fd;
};

class siohandler_t : public iohandler_t {
  public:
    siohandler_t(int d):_fd(d){}
	virtual ioevent_t* readfrom() {
        struct sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);
        int cfd = accept(_fd,(struct sockaddr*)&addr, &addrlen);
        if (cfd == -1) {
            if (errno == EINTR) {
                return NULL;
            } else {
                fprintf(stderr, "accept fail,error:%s\n", strerror(errno));
                return NULL;
            }
        }
        fprintf(stdout, "accept a new client: %s:%d\n", inet_ntoa(addr.sin_addr), addr.sin_port);

        ioevent_t *e = new ioevent_t(cfd, EV_CONN);
	    return e;
	}
	virtual int writeto() {return 0;}
  private:
	int _fd;
};


class server_engine_t {
  public:
    server_engine_t(const char *_ip, const int _port) :ip(_ip),port(_port) {
        FD_ZERO(&fdset);
		running = false;
    }
    int start();

  private:
    int _build();
    int _loop();

    int _set_prepare(struct timeval *tv);
    int _get_ready(int *fds, int n);
    int _get_ready(int *ready_fds, int *ready_n, struct timeval *tvp);
    iohandler_t* _get_handler(int fd);
    int _del_handler(int fd);
    int _on_event(ioevent_t *ev);

    int conn_nums;
    set<int> fds;
    fd_set fdset;
    int maxfd;

    const char *ip;
    const int port;
    bool running;

    hash_map<int, iohandler_t*> handlers;
};
