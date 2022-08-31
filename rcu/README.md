## RCU(Read-Copy Update) API
 - void rcu_read_lock(void)   //reader enter into critical section
 - void rcu_read_unlock(void) //reader exit from critical section
 
 - void rcu_dereference(p)  //reader acquire memory pointer for sharing resource
 - void rcu_assign_pointer(p, v)  //writer use new pointer replace old pointer(including memory barrier)
 
 - void synchronize_rcu(void)  //writer wait for ongoing readers finishing read-operation (grace period)
 - void call_rcu(head, callback_func);  //don't wait like synchronize_rcu, asyncronizely execute

## Reference
 - http://www2.rdrop.com/users/paulmck/RCU/
