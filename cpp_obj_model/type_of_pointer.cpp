#include <stdio.h>
#include <string>

class ZooAnimal{
public:
    ZooAnimal(const std::string &s):
        loc(1),
        name(s){
    }
    virtual ~ZooAnimal(){}
    virtual void rotate(){printf("ZooAnimal rotate.\n");}

protected:
    int loc;
    std::string name;
};

class Bear: public ZooAnimal{
public:
    Bear(const std::string &s):
        ZooAnimal(s),
        cell_block(1),
        known(Run) {
    }
    ~Bear(){}

    void rotate() override {printf("Bear rotate.\n");}
    void dance(){printf("Bear dance.\n");}
protected:
    enum Dances{Jump=1, Run=2, Dive=3};
    Dances known;
    int cell_block;
    friend void test();
};

void test(){
    Bear b("Yogi");
    ZooAnimal *pz = &b;
    Bear *pb = &b;

    // 不合法：不是ZooAnimal的成员
    // pz->cell_block;
    // pz->dance();
    
    pz->rotate();

    // ok:经过一个显示的downcast操作就没问题
    (static_cast<Bear*>(pz))->cell_block;

    // 下面这样更好，但它是一个runtime operator（成本更高）
    if (Bear* pb2 = dynamic_cast<Bear*>(pz)){
        pb2->cell_block;
    }

    // OK: cell_block是Bear的一个member
    pb->cell_block;
}

int main(){
    test();
    return 0;
}
