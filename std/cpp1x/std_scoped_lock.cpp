#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

int main ( int argc, char **argv ) {
    const int Num = 1000;
    std::mutex mtx1, mtx2;

    std::thread th1([&] {
        for ( int i = 0; i < Num; ++ i ){
            std::cout << "th1 single step begin i = " << i << std::endl;
            std::cout << "th1 std::scoped_lock grd(mtx1, mtx2)"<< std::endl;
            std::scoped_lock grd(mtx1, mtx2);

            std::cout << "th1 single step end\n";
        }
    });
    std::thread th2([&] {
        for (int i = 0; i < Num; ++ i ) {
            std::cout << "th2 single step begin i = " << i << std::endl;
            std::cout << "th2 std::scoped_lock grd(mtx2, mtx1)" << std::endl;
            std::scoped_lock grd(mtx2, mtx1);
            std::cout << "th2 single step end" << std::endl;
        }
    });

    th1.join();
    th2.join();
    return 0;
}
