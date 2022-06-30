/* （linux kernel, include/linux/compiler-gcc.h）*/
/* The "volatile" is due to gcc bugs */
#define barrier() __asm__ __volatile__("": : :"memory")

//GCC EXTEND: asm volatile("" ::: "memory");

/*The optimization barrier tells the compiler:
*   1. The memory has been modified and the register behind the barrier must be fetched from memory again
*   2. Generate in code order and must not cross barriers
*/
