#ifndef _PROCESS_BIND_CPU_H_
#define _PROCESS_BIND_CPU_H_

#include <sched.h>
#include <stdio.h>

int process_bind_cpu(const int cpu_id) {
    cpu_set_t mask;
    cpu_set_t get;

    CPU_ZERO(&mask);
    CPU_SET(cpu_id, &mask);

    if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
        printf("WARNING: Could not set CPU Affinity.\n");
        return -1;
    } else {
        printf("Sucess bind process to cpu id: %d.\n", cpu_id);
        return 0;
    }
}

#endif
