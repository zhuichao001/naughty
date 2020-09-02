#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct cow_string{
    static const int EXTEND_SIZE = 7; 
    static const int SIZE_OFFSET = 6; 
    static const int REFC_OFFSET = 2;

    char *_body;
    cow_string(){
        _body = (char*)malloc(0+EXTEND_SIZE);
        _body[0] = '\0';
        _body += SIZE_OFFSET;
        _initRef(1);
    }

    cow_string(const char* s){
        _body = NULL;
        operator=(s);
    }

    cow_string(const cow_string &s){
        _body = s._body;
        _incRef();
    }

    ~cow_string(){
        _decRef();
        if(_refCount()==0){
            delete [](_body-SIZE_OFFSET);
            _body = NULL;
        }
    }

    char &operator [](int i){
        if(i>=_strSize()){
           printf("Error, cow_string::operator[] index out of bound.\n");
           abort();
        }
        if(_refCount()>1){
            operator=(_body);
        }
        return _body[i];
    } 


    cow_string& operator=(const char *s){
        if(s==NULL){
           abort();
        }

        if(_body!=NULL){
            _decRef();
            if(_refCount()==0){
                delete [](_body-SIZE_OFFSET);
                _body = NULL;
            }
        }

        int len = strlen(s);
        _body = (char*)malloc(len+EXTEND_SIZE);
        *(int*)(_body) = len;
        _body += SIZE_OFFSET;
        strcpy(_body, s);
        _initRef(1);
        return *this;
    }

    cow_string& operator=(const cow_string &s){
        _body = s._body;
        _incRef();
    }

    const char* c_str() const {
        return _body;
    }

    int _refCount()const{ return int(*(short*)(_body-REFC_OFFSET)); }
    int _strSize()const{ return int(*(int*)(_body-SIZE_OFFSET)); }
    void _initRef(int n){ *(short*)(_body-REFC_OFFSET) = n; }
    void _incRef(){ ++*(short*)(_body-REFC_OFFSET); }
    void _decRef(){ --*(short*)(_body-REFC_OFFSET); }
    void _release(){
        delete [](_body-SIZE_OFFSET);
        _body = NULL;
    }
};
