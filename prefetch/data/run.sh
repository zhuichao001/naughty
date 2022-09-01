gcc test_prefetch.c -DDO_PREFETCH -o with-prefetch -std=c11 -O3
gcc test_prefetch.c -o no-prefetch -std=c11 -O3
perf stat -e L1-dcache-load-misses,L1-dcache-loads ./with-prefetch 
perf stat -e L1-dcache-load-misses,L1-dcache-loads ./no-prefetch 
