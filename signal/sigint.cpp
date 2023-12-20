#include <iostream>
#include <csignal>
#include <unistd.h>

void signalHandler(int signum)
{
    std::cout << "Interrupt signal (" << signum << ") recevied." << std::endl;
    exit(signum);
}

int main(int argc, char* argv[])
{
    int i = 0;
    signal(SIGINT, signalHandler);

    while(++i){
        std::cout << "Going to sleep "<< i <<"..." << std::endl;
        if(i == 3){
            raise(SIGINT);
        }
        sleep(1);
    }

    return 0;
}
