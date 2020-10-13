#include <string.h>
#include <string>
#include <iostream>

std::string strErrno(const int errnum) {
    thread_local static char errbuf[256] = {'\0'};
    return std::string(::strerror_r(errnum, errbuf, 255));
}

int main(){
    std::cout<<"[strerror] of 23: "<<strErrno(23)<<std::endl;
    std::cout<<"[strerror] of 28: "<<strErrno(28)<<std::endl;
    return 0;
}
