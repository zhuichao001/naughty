#include <future>
#include <iostream>
#include <unistd.h>

int find_the_answer(){
    //do some heavy calculation
    sleep(2);
    return 0;
}

void do_other_stuff(){
    sleep(1);
}

int main() {
    std::future<int> answer=std::async(find_the_answer);
    do_other_stuff();
    std::cout<<"The answer is "<<answer.get()<<std::endl;
    return 0;
}
