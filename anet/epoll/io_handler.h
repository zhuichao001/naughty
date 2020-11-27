#ifndef _NET_IOHANDLER_H_
#define _NET_IOHANDLER_H_

class io_handler_t {
public:
    virtual int open(){}
    virtual int close(){}
    virtual int read(){}
    virtual int write(){}
    virtual int handle(){}
    virtual ~io_handler_t(){}
};

#endif
