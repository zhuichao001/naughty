#include <iostream>

struct Base {
public:
    Base(){
        std::cout << "default construct, id:" << id << std::endl;
    }

    Base(int t):
        id(t){
        std::cout << "parameter construct, id:" << id << std::endl;
    }

    ~Base(){
        std::cout << "destroy, id:" << id << std::endl;
    }

    void* operator new(/*size为字节大小*/ size_t size /* , 其它参数 */) {
        std::cout << "operator new, size: " << size << std::endl;
        // operator new 需指定全局域，否则会调用自己的new，形成递归
        void* ptr = ::operator new(size);
        return ptr;
    }

    void print(){
        std::cout << "id:" << id << std::endl;
    }

    int id=0;
};
