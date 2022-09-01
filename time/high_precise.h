
inline unsigned long long rdtscp1(){
    unsigned int lo, hi;
    __asm volatile(
        "CPUID\n\t"
        "RDTSC\n\t"
        :"=d" (hi), "=a" (lo)
        :
        :"%ebx", "%ecx");
    return ((unsigned long long)lo) | (((unsigned long long)hi) << 32);
}

inline unsigned long long rdtscp2(){
    unsigned int lo, hi;
    __asm volatile(
        "RDTSCP\n\t"
        "MOV %%edx,%0\n\t"
        "MOV %%eax,%1\n\t"
        "CPUID\n\t"
        :"=r" (hi), "=r" (lo)
        :
        :"%eax", "%ecx", "%edx");
    return ((unsigned long long)lo) | (((unsigned long long)hi) << 32);
}
