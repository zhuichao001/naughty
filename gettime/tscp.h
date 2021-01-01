#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>


static unsigned long long get_tsc(void) {
    register uint32_t low, high;
    register unsigned long long t;
    __asm__  __volatile__  ("rdtscp" : "=a"(low), "=d"(high)::"%rcx");
    t = high;
    t <<= 32;
    return (t | low);

}

static unsigned long long get_cpu_khz() {
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if(!fp) return 1;
    char buf[2048] = {0};
    fread(buf,1, sizeof(buf), fp);
    fclose(fp);

    char *lp = strstr(buf, "cpu MHz");
    if(!lp) return 1;
    lp += strlen("cpu MHz");
    while(*lp == ' ' || *lp == '\t' || *lp == ':'){
        ++lp;
    }

    double mhz = atof(lp);
    return (unsigned long long)(mhz * 1000);
}

unsigned long long get_tick_ms() {
    static uint32_t khz = get_cpu_khz();
    return get_tsc() / khz;
}
