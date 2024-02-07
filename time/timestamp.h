
uint64_t now_ns(){
    uint64_t ns =
        std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now().time_since_epoch()).count();
    return ns;
}
