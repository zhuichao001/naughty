#include <iostream>
#include <semaphore.h>
#include <unistd.h>
#include <memory>
#include <thread>

sem_t firstdone;
sem_t seconddone;

class Sequential : public std::enable_shared_from_this<Sequential> {
    int v;
public:
    Sequential() {
        sem_init(/*sem_t *sem*/&firstdone, /*int pshared*/1, /*unsigned int value*/0);
        sem_init(&seconddone, 1, 0);
    }

    void first_step() {
        sleep(2);
        v = 1;
        sem_post(&firstdone);   // Release resource, increase the semaphore by 1 to indicate that the resource is free
        std::cout<< "first step v:" << v << std::endl;
    }

    void second_step() {
        sem_wait(&firstdone);   // Wait semaphore>0, a semaphore of 0 means no resources avaliable, so must wait until the semaphore > 0
        sleep(1);
        v = 2;
        std::cout<< "second step v:" << v << std::endl;
        sem_post(&seconddone);
    }

    void third_step() {
        sem_wait(&seconddone);
        v = 3;
        std::cout<< "third step v:" << v << std::endl;
    }

    std::shared_ptr<Sequential> get_this() {
        return shared_from_this();
    }
};

int main() {
    std::shared_ptr<Sequential> seqptr = std::make_shared<Sequential>();
    std::thread t1{ &Sequential::first_step,  *seqptr };
    std::thread t2{ &Sequential::second_step, *seqptr };
    std::thread t3{ &Sequential::third_step,  *seqptr };
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
