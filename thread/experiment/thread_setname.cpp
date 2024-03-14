#include <string>
#include <thread>
#include <unistd.h>

void run(){
    while(true){
        sleep(1);
    }
}


int main(){
    auto t1 = std::thread(run);
    auto t2 = std::thread(run);
    std::string name1 = "Thread 1";
    std::string name2 = "Thread 2";
    pthread_setname_np(t1.native_handle(), name1.substr(0, 15).c_str()); 
    pthread_setname_np(t2.native_handle(), name2.substr(0, 15).c_str()); 
    t1.join();
    t2.join();
    return 0;
}
