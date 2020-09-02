#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct eager_string{
    static const int EXTEND_SIZE = 5; 
    static const int SIZE_OFFSET = 4; 

    char *_body;

    eager_string(){
        _body = (char*)malloc(0+EXTEND_SIZE);
        _body[0] = '\0';
        _body += SIZE_OFFSET;
    }

    eager_string(const char* s){
        _body = NULL;
        operator=(s);
    }

    eager_string(const eager_string &s){
        _body = NULL;
        operator=(s._body);
    }

    ~eager_string(){
        delete [](_body-SIZE_OFFSET);
        _body = NULL;
    }

    char &operator [](int i){
        if(i>=_strSize()){
           printf("Error, eager_string::operator[] index out of bound.\n");
           abort();
        }
        return _body[i];
    } 

    eager_string& operator=(const char *s){
        if(s==NULL){
           printf("Error, eager_string::operator= src must not be NULL.\n");
           abort();
        }

        if(_body!=NULL){
            delete [](_body-SIZE_OFFSET);
            _body = NULL;
        }

        int len = strlen(s);
        _body = (char*)malloc(len+EXTEND_SIZE);
        *(int*)(_body) = len;
        _body += SIZE_OFFSET;
        strcpy(_body, s);
        return *this;
    }

    eager_string& operator=(const eager_string &s){
        delete [](_body-SIZE_OFFSET);
        operator=(s._body);
    }

    const char* c_str() const {
        return _body;
    }

    int _strSize()const{ return int(*(int*)(_body-SIZE_OFFSET)); }
};
