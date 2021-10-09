#ifndef _SMART_PTR_H_
#define _SMART_PTR_H_

class ref_count{
    int _shared_cnt;
    int _weak_cnt;
public:
    ref_count():
        _shared_cnt(0),
        _weak_cnt(0) {
    }

    ref_count & operator=(const ref_count &rc) {
        _shared_cnt = rc._shared_cnt;
        return *this;
    }

    int shared_count() {
        return _shared_cnt;
    }

    int weak_count() {
        return _weak_cnt;
    }

    int inc_shared() {
        return ++_shared_cnt;
    }

    int dec_shared() {
        return --_shared_cnt;
    }

    int inc_weak() {
        return ++_weak_cnt;
    }

    int dec_weak() {
        return --_weak_cnt;
    }
};


template<class T>
class shared_ptr{
    template<class> class weak_ptr;
    friend class weak_ptr<T>;
public:
    shared_ptr(T* p = nullptr) //TODO: support deleter
        :_px(p)
        ,_pn(new ref_count()) {
        if(p!=nullptr){
            _pn->inc_shared();
        }
    }

    ~shared_ptr() {
        release();
    }

    shared_ptr(const shared_ptr<T>& r)
        :_px(r._px)
        ,_pn(r._pn) {
        r._pn->inc_shared();
    }

    shared_ptr(const weak_ptr<T>& w)
        :_px(w._px)
        ,_pn(w._pn) {
        _pn->inc_shared();
    }

    shared_ptr<T>& operator=(const shared_ptr<T>& r) {
        if(this!=&r){
            release();
            if(r._px!=nullptr){
                r._pn->inc_shared();
            }
            _px = r._px;
            _pn = r._pn;
        }
        return *this;
    }

    T& operator*() {
        return *_px;
    }

    T* operator->() {
        return _px;
    }

    int use_count() { 
        return _pn==nullptr? 0 : _pn->shared_count(); 
    }

    T* get() { 
        return _px; 
    }

    bool unique() {
        return _pn->shared_count() == 1;
    }

private:
    void release() {
        if(use_count()==0){
            return;
        }

        _pn->dec_shared();
        if(_pn->shared_count()==0){
            delete _px;
            _px = nullptr;

            if(_pn->weak_count()==0){
                delete _pn;
            }
            _pn = nullptr;
        }
    }

    ref_count *_pn;
    T *_px;
};


template<class T>
class weak_ptr{
    friend class shared_ptr<T>;
public:
    weak_ptr():
        _px(nullptr),
        _pn(nullptr) {
    }

    ~weak_ptr() {
        release();
    }

    weak_ptr(const shared_ptr<T> &r): 
        _px(r._px),
        _pn(r._pn) {
        _pn->inc_weak();
    }

    weak_ptr(const weak_ptr<T> &w): 
        _px(w._px),
        _pn(w._pn) {
        _pn->inc_weak();
    }

    weak_ptr<T> & operator =(weak_ptr<T> &w) {
        if(this!=&w) {
            release();
            w._pn->inc_weak();
            _pn = w._pn;
            _px = w._px;
        }
        return *this;
    }

    weak_ptr<T> & operator =(shared_ptr<T> &r) {
        release();
        r._pn->inc_weak();
        _pn = r._pn;
        _px = r._px;
        return *this;
    }

    int use_count() const {
        return _pn==nullptr? 0 : _pn->shared_count();
    }

    bool expired() const {
        return _pn==nullptr? true : use_count()==0;
    }
    
    T& operator*() {
        return *_px;
    }

    T* operator->() {
        return _px;
    }

    shared_ptr<T> lock() const {
        return shared_ptr<T>(*this);
    }

    void release() {
        if(_pn==nullptr){
            return;
        }

        _pn->dec_weak();
        if(_pn->weak_count()==0 && _pn->shared_count()==0){
            delete _pn;
            _pn = nullptr;
        }
    }

private:
    T *_px;
    ref_count *_pn;
};


#endif
