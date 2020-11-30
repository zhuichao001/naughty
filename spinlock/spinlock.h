#include <atomic>


class spinlock {
public:
    spinlock(void) throw() : locked(ATOMIC_VAR_INIT(false)) { }
    spinlock(const spinlock &) throw() = delete;
    spinlock &operator=(const spinlock &) throw() = delete;
    ~spinlock(void) throw() = default;

    inline void acquire(void) throw() {
        while (true) {
            if (locked.load(std::memory_order_acquire)) {
                continue;
            }
            if (!locked.exchange(true, std::memory_order_acquire)) {
                break;
            }
        }
    }

    inline void release(void) throw() {
        locked.store(false, std::memory_order_release);
    }

private:
    std::atomic<bool> locked;
};
