#ifndef _NAUGHTY_ATOMIC_H_
#define _NAUGHTY_ATOMIC_H_


typedef struct {  
    volatile int counter;  
}atomic_t;

struct __xchg_dummy { unsigned long a[100]; };
#define __xg(x) ((struct __xchg_dummy *)(x))

#define ATOMIC_INIT(i) {(i)}

#define LOCK_PREFIX \
    ".section .smp_locks,\"a\"\n" \
    ".align 4\n" \
    ".long 661f\n" \
    ".previous\n" \
    "661:\n\tlock; "

static inline int atomic_read(const atomic_t *v){  
    return v->counter;  
}  
  
static inline void atomic_set(atomic_t *v, int i){  
    v->counter = i;  
}

static inline void atomic_add(int i, atomic_t *v){  
    asm volatile(LOCK_PREFIX "addl %1,%0"  
            : "+m" (v->counter)  
            : "ir" (i));  
}

static inline void atomic_sub(int i, atomic_t *v){  
    asm volatile(LOCK_PREFIX "subl %1,%0"  
            : "+m" (v->counter)  
            : "ir" (i));  
}

static inline int atomic_add_return(int i, atomic_t *v){
    int __i = i;
    asm volatile(LOCK_PREFIX "xaddl %0, %1"
            : "+r" (i), "+m" (v->counter)
            : : "memory");
    return i + __i;
}

static inline int atomic_sub_return(int i, atomic_t *v){  
    return atomic_add_return(-i, v);
}

static inline void atomic_inc(atomic_t *v){  
    asm volatile(LOCK_PREFIX "incl %0"
            : "+m" (v->counter));
}
  
static inline void atomic_dec(atomic_t *v){  
    asm volatile(LOCK_PREFIX "decl %0"
            : "+m" (v->counter));
}

static inline int atomic_inc_and_test(atomic_t *v){
    unsigned char c;
    asm volatile(LOCK_PREFIX "incl %0; sete %1"
            : "+m" (v->counter), "=qm" (c)
            : : "memory");
    return c != 0;
}

static inline int atomic_dec_and_test(atomic_t *v){
    unsigned char c;
    asm volatile(LOCK_PREFIX "decl %0; sete %1"
            : "+m" (v->counter), "=qm" (c)
            : : "memory");
    return c != 0;
}

static inline int atomic_add_and_test(int i, atomic_t *v){  
    unsigned char c;  
    asm volatile(LOCK_PREFIX "addl %2,%0; sete %1"  
            : "+m" (v->counter), "=qm" (c)  
            : "ir" (i) : "memory");  
    return c;  
}

static inline int atomic_sub_and_test(int i, atomic_t *v){  
    unsigned char c;  
    asm volatile(LOCK_PREFIX "subl %2,%0; sete %1"  
            : "+m" (v->counter), "=qm" (c)  
            : "ir" (i) : "memory");  
    return c;  
}

static inline int atomic_add_negative(int i, atomic_t *v){
    unsigned char c;
    asm volatile(LOCK_PREFIX "addl %2,%0; sets %1"
            : "+m" (v->counter), "=qm" (c)
            : "ir" (i) : "memory");
    return c;
}

static inline unsigned long __cmpxchg(volatile void *ptr, unsigned long old,  unsigned long new, int size){  
    unsigned long prev;
    switch(size){
        case 1:
            asm volatile(LOCK_PREFIX "cmpxchgb %b1,%2"  
                    : "=a"(prev)  
                    : "q"(new), "m"(*__xg(ptr)), "0"(old)  
                    : "memory");  
            return prev;  
        case 2:  
            asm volatile(LOCK_PREFIX "cmpxchgw %w1,%2"  
                    : "=a"(prev)  
                    : "r"(new), "m"(*__xg(ptr)), "0"(old)  
                    : "memory");  
            return prev;  
        case 4:  
            asm volatile(LOCK_PREFIX "cmpxchgl %k1,%2"  
                    : "=a"(prev)  
                    : "r"(new), "m"(*__xg(ptr)), "0"(old)  
                    : "memory");  
            return prev;  
        case 8:  
            asm volatile(LOCK_PREFIX "cmpxchgq %1,%2"  
                    : "=a"(prev)  
                    : "r"(new), "m"(*__xg(ptr)), "0"(old)  
                    : "memory");  
            return prev;  
    }  
    return old;  
}  

#define cmpxchg(ptr, o, n) ((__typeof__(*(ptr)))__cmpxchg((ptr), (unsigned long)(o), (unsigned long)(n), sizeof(*(ptr))))  

static inline int atomic_cmpxchg(atomic_t *v, int old, int new){  
    return cmpxchg(&v->counter, old, new);
}


static inline unsigned long __xchg(unsigned long x, volatile void *ptr, int size){  
    switch (size) {  
        case 1:  
            asm volatile("xchgb %b0,%1"  
                    : "=q" (x)  
                    : "m" (*__xg(ptr)), "0" (x)  
                    : "memory");  
            break;  
        case 2:  
            asm volatile("xchgw %w0,%1"  
                    : "=r" (x)  
                    : "m" (*__xg(ptr)), "0" (x)  
                    : "memory");  
            break;  
        case 4:  
            asm volatile("xchgl %k0,%1"  
                    : "=r" (x)  
                    : "m" (*__xg(ptr)), "0" (x)  
                    : "memory");  
            break;  
        case 8:  
            asm volatile("xchgq %0,%1"  
                    : "=r" (x)  
                    : "m" (*__xg(ptr)), "0" (x)  
                    : "memory");  
            break;  
    }  
    return x;  
}

#define xchg(ptr, v) ((__typeof__(*(ptr)))__xchg((unsigned long)(v), (ptr), sizeof(*(ptr))))

static inline int atomic_xchg(atomic_t *v, int new){
    return xchg(&v->counter, new);
}


#endif
