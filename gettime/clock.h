#include <time.h>


long get_time_sec(){
    static struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec;
}

long get_time_nsec(){
    static struct timespec ts;
    /*
    CLOCK_REALTIME
        System-wide realtime clock. Setting this clock requires appropriate privileges.
    CLOCK_MONOTONIC
        Clock that cannot be set and represents monotonic time since some unspecified starting point.
    CLOCK_PROCESS_CPUTIME_ID
        High-resolution per-process timer from the CPU.
    CLOCK_THREAD_CPUTIME_ID
        Thread-specific CPU-time clock.
    */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
    return ts.tv_nsec;
}
