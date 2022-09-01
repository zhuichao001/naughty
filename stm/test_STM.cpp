
#include <iostream> 
#include <thread>

static const auto THR_NUM = 4;
static const auto ITER_NUM = 1000 * 1000;

static auto a = 0, b = 0,c = 0;

static void func() {
    for (auto i = 0; i < ITER_NUM; ++i) {
        __transaction_atomic {
            ++a;
            b += 2;
            c = a + b;
        }
    }
}

int main(int argc, char *argv[]) {
    std::thread thr[THR_NUM];

    for (auto &t : thr) {
        t = std::thread(func);
    }

    for (auto &t : thr) {
        t.join();
    }

    std::cout << "a=" << a << " b=" << b << " c=" << c << std::endl;

    return 0;
}
