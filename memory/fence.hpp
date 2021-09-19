
template <typename T>
bool CompareAndSwap(T* object, T expected, T desired) {
    // TODO: use c11 atomic_compare_exchange_weak
    return __sync_bool_compare_and_swap(object, expected, desired);
}


void SpinWait() {
        asm volatile("pause" : : : "memory");
}

void Fence() {
        asm volatile("" : : : "memory");
}

void AcquireFence() {
        asm volatile("" : : : "memory");
}

void ReleaseFence() {
        asm volatile("" : : : "memory");
}

void MemoryFence() {
        asm volatile("mfence" : : : "memory");
}
