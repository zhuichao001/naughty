#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>

using namespace std;

namespace naughty{

template<class T>
inline T* _allocate(ptrdiff_t size, T*) {
    set_new_handler(0);
    T* p = (T*)(::operator new((size_t)(size*sizeof(T))));
    if (p == 0){
        cerr << "Out Of Memory" << endl;
        exit(1);
    }
    return p;
}

template<class T>
inline void _deallocate(T* ptr) {
    ::operator delete(ptr);
}

template<class T, class R>
inline void _construct(T* ptr, const R& value) {
    new(ptr) T(value);
}

template<class T>
inline void _destroy(T* ptr) {
    ptr->~T();
}


template<class T>
class allocator {
public:
    typedef T   value_type;
    typedef T*  pointer;
    typedef const T* const_pointer;
    typedef T&  reference;
    typedef const T& const_reference;
    typedef size_t  size_type;
    typedef ptrdiff_t difference_type;

    //rebind allocator of type U
    template<class U>
    struct rebind {
        typedef allocator<U> other;
    };

    //hint used for locality
    pointer allocate(size_type n, const void* hint=nullptr) {
        return _allocate((difference_type)n, (pointer)nullptr);
    }

    void deallocate(pointer p, size_type n) { 
        _deallocate(p);
    }

    void construct(pointer p, const T& value) {
        _construct(p, value);
    }

    void destroy(pointer p) {
        _destroy(p);
    }

    pointer address(reference x) {
        return (pointer)&x;
    }

    const_pointer const_address(const_reference x) {
        return (const_pointer)&x;
    }

    size_type max_size()const{
        return size_type(UINT_MAX/sizeof(T));
    }
};

}//end of namespace naughty

#endif
