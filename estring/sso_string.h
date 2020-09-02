#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct sso_string{
    static const int Capacity = 16;
    int size;
    char data[Capacity];
    sso_string():size(0){}

    sso_string(const char* s){
        operator=(s);
    }

    char & operator [](int i){
        if(i>=size){
           printf("Error, sso_string::operator[] index out of bound.\n");
           abort();
        }
        return data[i];
    } 

    sso_string operator= (const char *s){
        size = 0;
        while(s!=NULL && *s!=0 &&size<Capacity-1){
            data[size] = *s;
            size += 1;
            s += 1;
        }
        data[size] = '\0';
        return *this;
    }

    const char* c_str() const {
        return data;
    }
};
