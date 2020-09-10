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

const int MAX_CONN_NUMS    = 1024;

const int EV_NIL         = 0;
const int EV_DATA_IN     = 1;
const int EV_DATA_OUT    = 2; 
const int EV_CONN        = 4;
const int EV_DISCONN     = 8;


class buff_t {
  public:
    buff_t():_capacity(1024),_size(0){
        _buff = new char[_capacity];
    }

    ~buff_t(){
        delete []_buff;
    }

    void append(const char *data, const int len){
		if(_size+len>_capacity){
			_buff = (char*)realloc(_buff, _capacity<<1);
		}
		memcpy(_buff+_size, data, len);
        _size += len;
    }

   	void clear(int n) {
        if(_size-n>0){
            memcpy(_buff, _buff+n, _size-n);
        }
		_size -= n;
	}

    void get(char* &data, int &len){
		data = _buff;
		len = _size;
	}

    char* getdata(){
        return _buff;
    }

    bool empty(){
        return _size==0;
    }
  private:
    char *_buff;
    int _capacity;
    int _size;    
};


class ioevent_t {
  public:
    ioevent_t(int t, int d){type=t; fd=d;}    
    int type;
    int fd;
};


class iohandler_t {
  public:
    iohandler_t(int d):fd(d){}

    ioevent_t* readfrom() {
        const int TMP_LEN = 1024;
        char tmp[TMP_LEN]={0,};
        ioevent_t *e = new ioevent_t(EV_NIL, fd);
        while(true) {
            int n = read(fd, tmp, TMP_LEN);
            if (n<0 &&errno==EAGAIN) {// read done
                break;
            } else if(n==0){ //conn closed
                close(fd);
                e->type |= EV_DISCONN;
                fprintf(stderr,"client fd=%d is closed.\n", fd);
                break;
            } else if(n<0){ //read error
                close(fd);
                e->type |= EV_DISCONN;
                fprintf(stderr,"client: read errno:%d.\n", errno);
                break;
            } else { //normal
                e->type |= EV_DATA_IN;
                fprintf(stdout,"client fd=%d read :%s.\n", fd, tmp);
                rbuff.append(tmp, n);
                if(n<TMP_LEN){
                    break;
                }
            }
        }
        return e;
    }

    ioevent_t* writeto() {
        ioevent_t *e = new ioevent_t(EV_NIL, fd);
        while(!wbuff.empty()){
            char *wdata = NULL;
            int wlen = 0;
            wbuff.get(wdata, wlen); 
            int n = write(fd, wdata, wlen);
            if (n<0 && errno == EAGAIN) { //tcp buffer is full
                break;
            } else if(n<=0) { //error
                e->type |= EV_DISCONN;
                fprintf(stderr,"client: write errno:%d.\n", errno);
                break;
            } else {
                e->type |= EV_DATA_OUT;
                wbuff.clear(n);
            }
        }
        return e;
    }
    bool rbuff_empty() {return rbuff.empty();};
    bool wbuff_empty() {return wbuff.empty();};

    int fd;
    buff_t rbuff;
    buff_t wbuff;
};


class fd_set_t {
  public:
    fd_set_t(){
        maxfd = 0;
        FD_ZERO(&r);
        FD_ZERO(&w);
    }

    void up_maxfd(int fd){
        if(fd>maxfd){
            maxfd = fd;
        }
    }

    void add_r(int fd){
        up_maxfd(fd);
        allfds.insert(fd);
        FD_SET(fd, &r);
    }

    void del_r(int fd){
        allfds.erase(fd);
        FD_CLR(fd, &r);
    }

    void add_w(int fd){
        up_maxfd(fd);
        allfds.insert(fd);
        FD_SET(fd, &w);
    }

    void del_w(int fd){
        allfds.erase(fd);
        FD_CLR(fd, &w);
    }

    void del(int fd){
        allfds.erase(fd);
        FD_CLR(fd, &r);
        FD_CLR(fd, &w);
    }

    int get_maxfd(){
        return maxfd;
    }

    bool readable(int fd){
        return FD_ISSET(fd, &_r);
    }

    bool writeable(int fd){
        return FD_ISSET(fd, &_w);
    }

    int maxfd;
    set<int> allfds;
    fd_set r, w;
    fd_set _r, _w;
};


class server_engine_t {
  public:
    server_engine_t(const char *_ip, const int _port) :ip(_ip),port(_port) {
    }
    int start();

  private:
    int _init_socket();
    int _loop();

    int _get_ready(vector<int> &ready_r, vector<int> &ready_w);
    iohandler_t* _get_handler(int fd);
    int _del_handler(int fd);
    int _add_handler(int fd);
    int _on_event(ioevent_t *ev, iohandler_t *h);

    int _accept() {
        fprintf(stderr, "<to accept> \n");
        struct sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);
        do {
            int cfd = accept(svrfd, (struct sockaddr*)&addr, &addrlen);
            if (cfd == -1) {
                if (errno==EINTR || errno==EAGAIN || errno==EPROTO || errno==ECONNABORTED) {
                    return -1;
                } else {
                    fprintf(stderr, "accept fail,error:%s\n", strerror(errno));
                    running = false;
                    return -1;
                }
            } else {
                fprintf(stdout, "accept: %s:%d, fd:%d\n", inet_ntoa(addr.sin_addr), addr.sin_port, cfd);
                fs.add_r(cfd);
                _add_handler(cfd); 
            } 
        }while(1);
        return 0;
    }

    int svrfd;
    fd_set_t fs;

    const char *ip;
    const int port;
    bool running;

    hash_map<int, iohandler_t*> handlers;
};
