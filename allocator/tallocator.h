#ifndef _NAUGHTY_ALLOCATOR_H_
#define _NAUGHTY_ALLOCATOR_H_

#include "tmalloc.h"

namespace naughty{

template<class T>
class tallocator {
public:
    typedef T   value_type_t;
    typedef T*  pointer_t;
    typedef const T* const_pointer_t;
    typedef T&  reference_t;
    typedef const T& const_reference_t;
    typedef size_t  size_type_t;
    typedef ptrdiff_t difference_type_t;

    pointer_t alloc(){
        return (pointer_t)tm.alloc(sizeof(T));
    }

    void dealloc(pointer_t p){
        return tm.dealloc(p);
    }

private:
    tmalloc tm;
};

}//end namespace naughty

#endif
