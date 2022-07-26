#include "skiplist.h"
#include <iostream>

using namespace std;

using Skiplist = skiplist_t<std::string, std::string>;

void test(){
    Skiplist sk(8, 4);
    sk.insert("u","85");
    sk.insert("c","3");
    sk.insert("d","4");
    sk.insert("i","8");
    sk.insert("e","5");
    sk.insert("f","6");
    sk.insert("y","98");
    sk.insert("z","198");
    sk.insert("x","89");
    sk.insert("k","9");
    sk.insert("e","5");
    sk.insert("f","6");
    sk.insert("y","98");
    sk.insert("k","9");
    sk.insert("b","2");
    sk.insert("y","98");
    sk.insert("z","198");
    sk.insert("x","89");
    sk.insert("k","9");
    sk.insert("b","2");
    sk.insert("y","98");
    sk.insert("z","198");
    sk.insert("x","89");
    sk.insert("k","9");
    sk.insert("p","18");
    sk.insert("q","81");
    sk.insert("g","7");
    sk.insert("h","8");
    sk.insert("j","8");
    sk.insert("k","8");
    sk.insert("a","1");
    sk.insert("v","87");
    sk.insert("w","88");
    sk.insert("r","82");
    sk.insert("s","83");
    sk.insert("t","84");
    sk.insert("m","10");
    sk.insert("n","11");
    sk.insert("o","12");
    sk.print();

    node_t<std::string, std::string> *t = sk.find("e");
    if (t==NULL) {
        cout << " not foud " << endl;
        return;
    }
    cout << t->key << " " << t->val << endl;
}

int main(){
    test();
    return 0;
}
