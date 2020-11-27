#ifndef _NET_BUFFER_H_
#define _NET_BUFFER_H_

#include <stdlib.h>
#include <memory.h>

class buff_t {
  public:
    buff_t(int cap):_capacity(cap), _offset(0) ,_size(0){
        _buff = new char[_capacity];
    }

    ~buff_t(){
        if(_buff!=NULL){
            delete []_buff;
        }
    }

    void append(const char *data, const int len){
        if(_size+len>_capacity){
            _buff = (char*)realloc(_buff, _capacity<<1);
        }
        memcpy(_buff+_size, data, len);
        _size += len;
    }

    void reset(){
        delete []_buff;
        _size = 0;
        _offset = 0;
        _capacity = 0;
    }

    void clear(int n) {
        if(_size-n>0){
            memcpy(_buff, _buff+n, _size-n);
        }
        _size -= n;
    }

    int get(char **data, int *len){
        *len = _capacity - _size - _offset;
        *data =  _buff + _offset;
        return 0;
    }

    bool empty(){
        return _size==0;
    }
private:
    char *_buff;
    int _capacity;
    int _size;    
    int _offset;
};

#endif
