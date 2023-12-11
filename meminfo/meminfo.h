#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<string.h>

struct MEM_INFO
{
    unsigned int total;
    unsigned int free;
    unsigned int buffers;
    unsigned int cached;
    unsigned int swap_cached;
    unsigned int swap_total;
    unsigned int swap_free;
    unsigned int available;
};
typedef struct MEM_INFO Mem_info;

void  get_mem_occupy (Mem_info *o)
{
    FILE* fpMemInfo = fopen("/proc/meminfo", "r");
    if (NULL == fpMemInfo)
    {
        return ;
    }
    int i = 0;
    int value;
    char name[1024];
    char line[1024];
    int nFiledNumber = 2;
    int nMemberNumber = 5;
    while (fgets(line, sizeof(line) - 1, fpMemInfo))
    {
        if (sscanf(line, "%s%u", name, &value) != nFiledNumber)
        {
            continue;
        }
        if (0 == strcmp(name, "MemTotal:"))
        {
            ++i;
            o->total = value;
        }
        else if (0 == strcmp(name, "MemFree:"))
        {
            ++i;
            o->free = value;
        }
        else if (0 == strcmp(name, "MemAvailable:"))
        {
            ++i;
            o->available = value;
        }
        else if (0 == strcmp(name, "Buffers:"))
        {
            ++i;
            o->buffers = value;
        }
        else if (0 == strcmp(name, "Cached:"))
        {
            ++i;
            o->cached = value;
        }
        if (i == nMemberNumber)
        {
            break;
        }
    }
    system("free");
    system("cat /proc/meminfo");
    printf("MemTotal      : %d\n",o->total);
    printf("MemFree       : %d\n",o->free);
    printf("MemAvailable  : %d\n",o->available);
    printf("MemBuffers    : %d\n",o->buffers);
    printf("MemCached     : %d\n",o->cached);
    printf("MemSwapCached : %d\n",o->swap_cached);
    printf("MemSwapTotal  : %d\n",o->swap_total);
    printf("MemSwapFree   : %d\n",o->swap_free);
    fclose(fpMemInfo);
}

float cal_mem_occupy(Mem_info *o)
{
    return (100.0 * (o->total - o->available) / o->total);
}
