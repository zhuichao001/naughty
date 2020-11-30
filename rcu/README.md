## RCU(Read-Copy Update) API
 - static inline void rcu_read_lock(void)   //reader enter into critical section
 - static inline void rcu_read_unlock(void) //reader exit from critical section
 - rcu_dereference(p)  //reader acquire memory pointer for sharing resource
 - rcu_assign_pointer(p, v)  //writer use new pointer replace old pointer
 - void synchronize_rcu(void)  //writer wait for original reader finish read operation
