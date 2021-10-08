#ifndef _SMART_PTR_H_
#define _SMART_PTR_H_

class shared_count{
private:
    int _use_cnt;
public:
    shared_count():
        _use_cnt(1) {
    }

    shared_count & operator=(const shared_count &sc){
        _use_cnt = sc._use_cnt;
        return *this;
    }

    int use_count(){
        return _use_cnt;
    }

    int inc_ref(){
        return ++_use_cnt;
    }

    int dec_ref(){
        return --_use_cnt;
    }
};

template<class T>
class shared_ptr{
public:
    shared_ptr(T* ptr = nullptr) //TODO: support deleter
        :_px(ptr)
        ,_pn(new shared_count()) {
        }

    ~shared_ptr() {
        release();
    }

    shared_ptr(const shared_ptr<T>& r)
        :_px(r._px)
        ,_pn(r._pn) {
    }

    shared_ptr<T>& operator=(const shared_ptr<T>& sp) {
        if(_pn!=sp._pn){
            if(sp._px!=nullptr){
                sp._pn->inc_ref();
            }
            if(_pn->use_count()!=0){
                delete _px;
                delete _pn;
            }
        }
        _px = sp._px;
        _pn = sp._pn;
        return *this;
    }

    T& operator*(){
        return *_px;
    }

    T* operator->(){
        return _px;
    }

    int use_count() { 
        return _pn->use_count(); 
    }

    T* get() { 
        return _px; 
    }

    bool unique(){
        return _pn->use_count() == 1;
    }

private:
    void release(){
        if(_pn->use_count()==0){
            return;
        }
        _pn->dec_ref();
        if(_pn->use_count()==0){
            delete _px;
            delete _pn;
        }
    }
    shared_count *_pn;
    T *_px;
};

#endif
