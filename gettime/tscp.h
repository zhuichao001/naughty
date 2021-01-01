#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <limits.h>


static unsigned long long get_tsc(void) {
    register uint32_t low, high;
    register unsigned long long o;
    __asm__  __volatile__  ("rdtscp" : "=a"(low), "=d"(high)::"%rcx");
    o = high;
    o <<= 32;
    return (o | low);

}


static unsigned long long get_cpu_khz() {
    FILE *fp = fopen("/proc/cpuinfo","r");
    if(!fp) return 1;
    char buf[4096] = {0};
    fread(buf,1,sizeof(buf),fp);
    fclose(fp);

    char *lp = strstr(buf,"cpu MHz");
    if(!lp) return 1;
    lp += strlen("cpu MHz");
    while(*lp == ' ' || *lp == '\t' || *lp == ':')
    {
        ++lp;
    }

    double mhz = atof(lp);
    unsigned long long u = (unsigned long long)(mhz * 1000);
    return u;
}


unsigned long long get_tick_ms() {
    static uint32_t khz = get_cpu_khz();
    return get_tsc() / khz;
}
