#include <string.h>
#include <string>
#include <iostream>

std::string strErrno(const int errnum) {
    thread_local static char errbuf[256] = {'\0'};
    char* ret = ::strerror_r(errnum, errbuf, 255);
    return std::string(ret);
}

int main(){
    std::cout<<"[strerror]: "<<strErrno(23)<<std::endl;
    return 0;
}
