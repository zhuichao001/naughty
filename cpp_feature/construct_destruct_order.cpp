#include <iostream>
#include <string.h>

class CDemo {
public:
    CDemo(const char* str); // constructor
    ~CDemo(); // destructor
private:
    char name[20];
};

CDemo::CDemo(const char* str) {// constructor
    strncpy(name, str, 20);
    std::cout << "Constructor called for " << name << '\n';
}

CDemo::~CDemo() {// destructor
    std::cout << "Destructor called for " << name << '\n';
}

void func() {
    CDemo LocalObjectInFunc("LocalObjectInFunc");
    static CDemo StaticObject("StaticObject");
    CDemo* pHeapObjectInFunc = new CDemo("HeapObjectInFunc");
    delete pHeapObjectInFunc;
    std::cout << "before return from Inside func" << std::endl;
}

CDemo GlobalObject("GlobalObject");

int main() {
    CDemo LocalObjectInMain("LocalObjectInMain");
    CDemo* pHeapObjectInMain = new CDemo("HeapObjectInMain");

    std::cout << "In main, before calling func\n\n";
    func();
    std::cout << "\nIn main, again calling func\n\n";
    func();
    std::cout << "\nIn main, after calling func\n";
    delete pHeapObjectInMain;
    return 0;
}
