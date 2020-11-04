#ifndef _SINGLETON_H_ 
#define _SINGLETON_H_ 

#include "noncopyable.h"

template <class T>
class singleton: public noncopyable {
private:
    static T* _inst;
public:
    singleton(){}
    virtual ~singleton(){}

    static T& instance(){
        if(_inst==nullptr){
            _inst = new T;
        }
        return *_inst;
    }

    static void remove(){
        if(_inst!=nullptr){
            delete _inst;
            _inst = nullptr;
        }
    }
};

template <class T>  
T * singleton<T>::_inst = nullptr;

#endif
