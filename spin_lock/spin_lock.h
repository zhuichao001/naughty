#include <atomic>


struct spin_lock {
    spin_lock(const spin_lock &) throw() = delete;
    spin_lock &operator=(const spin_lock &) throw() = delete;
    ~spin_lock(void) throw() = default;

    spin_lock(void) throw() : is_locked(ATOMIC_VAR_INIT(false)) { }

    inline void acquire(void) throw() {
        for(;;) {
            if(is_locked.load(std::memory_order_acquire)) {
                continue;
            }

            if(!is_locked.exchange(true, std::memory_order_acquire)) {
                break;
            }
        }
    }

    inline void release(void) throw() {
        is_locked.store(false, std::memory_order_release);
    }

  private:
    std::atomic<bool> is_locked;
};
