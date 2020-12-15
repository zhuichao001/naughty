#ifndef _NAUGHTY_OBJPOOL_H_
#define _NAUGHTY_OBJPOOL_H_

#include "tallocator.h"

namespace naughty{

template<class T>
class objpool {
public:
    T* fetch() {
        void *p = actor.alloc();
        return new((T*)p) T();
    }

    T* fetch(const T& value) {
        void *p = actor.alloc();
        return new((T*)p) T(value);
    }

    void putin(T* p){
        ((T*)p)->~T();
        actor.dealloc(p);
    }

private:
    tallocator<T> actor;
};

}//end namespace naughty

#endif
