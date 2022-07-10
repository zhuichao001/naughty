// advance example
#include <iostream>     // std::cout
#include <iterator>     // std::advance
#include <list>         // std::list

int main () {
    std::list<int> mylist;
    for (int i=0; i<10; i++) {
        mylist.push_back (i);
    }
    std::list<int>::iterator it = mylist.begin();
    std::advance (it,5);
    std::cout << "The sixth element in mylist is: " << *it << '\n';

    std::advance (it,20);
    std::cout << "The 20th element in mylist is: " << *it << '\n';
    return 0;
}
