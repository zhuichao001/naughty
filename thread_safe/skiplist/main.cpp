#include "skiplist.h"
#include <string>
#include <iostream>

using Skiplist = skiplist_t<std::string, std::string>;

void test(){
    Skiplist sk(8, 4);
    sk.put("u", "85");
    sk.put("c", "3");
    sk.put("d", "4");
    sk.put("i", "8");
    sk.put("e", "5");
    sk.put("f", "6");
    sk.put("y", "98");
    sk.put("z", "198");
    sk.put("x", "89");
    sk.put("k", "9");
    sk.put("e", "5");
    sk.put("f", "6");
    sk.put("y", "98");
    sk.put("k", "9");
    sk.put("b", "2");
    sk.put("y", "98");
    sk.put("z", "198");
    sk.put("x", "89");
    sk.put("k", "9");
    sk.put("b", "2");
    sk.put("y", "98");
    sk.put("z", "198");
    sk.put("x", "89");
    sk.put("k", "9");
    sk.put("p", "18");
    sk.put("q", "81");
    sk.put("g", "7");
    sk.put("h", "8");
    sk.put("j", "8");
    sk.put("k", "8");
    sk.put("a", "1");
    sk.put("v", "87");
    sk.put("w", "88");
    sk.put("r", "82");
    sk.put("s", "83");
    sk.put("t", "84");
    sk.put("m", "10");
    sk.put("n", "11");
    sk.put("o", "12");
    sk.print();

    std::string val;
    int err = sk.get("e", val);
    if (err==-1) {
        std::cout << " not foud " << std::endl;
        return;
    }
    std::cout << "found, e: " << val << std::endl;
}

int main(){
    test();
    return 0;
}
