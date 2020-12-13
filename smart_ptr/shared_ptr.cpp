#include <iostream>
#include <memory>

void get_shared_ptr(){
    int* p = new int (10);
    std::shared_ptr<int> a (p);

    if (a.get()==p) {
        std::cout << "a and p point to the same location\n";
    }

    // three ways of accessing the same address:
    std::cout << *a.get() << *a << *p << "\n";
}

void swap_shared_ptr(){
    std::shared_ptr<int> foo (new int(10));
    std::shared_ptr<int> bar (new int(20));

    foo.swap(bar);

    std::cout << "*foo: " << *foo << '\n';
    std::cout << "*bar: " << *bar << '\n';
}

void assign_shared_ptr(){
    std::shared_ptr<int> sp1(new int(22));
    std::shared_ptr<int> sp2 = sp1;
    std::cout << "cout: " << sp2.use_count() << std::endl;
    std::cout << "val: " << *sp1 << "|" << *sp2 << std::endl;

    sp1.reset(); 
    std::cout << "count: " << sp2.use_count() << std::endl;
    std::cout << "val: " << *sp2 << std::endl;
}

//bad case, assign stack addr to shared_ptr
void assign_stack_ptr(){
    std::string s("I am a string on stack.");
    std::shared_ptr<std::string> ps(&s);  //error: aborted
}

int main () {
    get_shared_ptr();
    swap_shared_ptr();
    assign_shared_ptr();
    assign_stack_ptr();
    return 0;
}
