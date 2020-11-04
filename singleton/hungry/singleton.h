#ifndef _SINGLETON_H_ 
#define _SINGLETON_H_ 

#include "noncopyable.h"

template <class T>
class singleton: public noncopyable {
public:
    singleton(){}
    virtual ~singleton(){}

    static T& instance(){
        return *_inst;
    }

    class releaser{
        ~releaser(){
            if(_inst!=nullptr){
                delete _inst;
                _inst = nullptr;
            }
        }
    };
private:
    static T* _inst;
    static releaser _;
};

template <class T>
T * singleton<T>::_inst = new T;

#endif
