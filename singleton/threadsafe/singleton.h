#ifndef _SINGLETON_H_ 
#define _SINGLETON_H_ 

#include "noncopyable.h"
#include <mutex>


template <class T>
class singleton: public noncopyable {
public:
    singleton(){}
    virtual ~singleton(){}

    static T& instance(){
        if(_inst==nullptr){
            std::lock_guard<std::mutex> lock(singleton::_mutex);
            if(_inst==nullptr){
                singleton::_inst = new T;
            }
        }
        return *_inst;
    }

    static void remove(){
        if(_inst!=nullptr){
            delete _inst;
            _inst = nullptr;
        }
    }
private:
    static T* _inst;
    static std::mutex _mutex;
};

template <class T>  
T * singleton<T>::_inst = nullptr;

template <class T>  
std::mutex singleton<T>::_mutex;

#endif
