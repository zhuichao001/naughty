#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <map>

using namespace std;

std::once_flag falg;

void do_once(){
    std::cout << "Call Before!" << endl;
    std::call_once(falg,[](){std::cout << "Calling...!" << std::endl;});
    std::cout << "Call After!" << endl;
}

int main(){
    std::thread t1(do_once);
    std::thread t2(do_once);
    std::thread t3(do_once);
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
