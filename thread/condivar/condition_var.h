//FIXME: pseudo code

typedef struct {
    int c_spinlock;                  // Spin lock to protect the queue.
    struct _pthread_queue c_waiting; // Threads waiting on this condition.
} pthread_cond_t;

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex) {
    volatile pthread_t self = thread_self();

    acquire(&cond->c_spinlock);
    enqueue(&cond->c_waiting, self);
    release(&cond->c_spinlock);

    pthread_mutex_unlock(mutex);
    suspend_with_cancellation(self);
    pthread_mutex_lock(mutex);
    if (self->p_canceled && self->p_cancelstate == PTHREAD_CANCEL_ENABLE) {
        // Remove ourselves from the waiting queue if we're still on it
        acquire(&cond->c_spinlock);
        remove_from_queue(&cond->c_waiting, self);
        release(&cond->c_spinlock);
        pthread_exit(PTHREAD_CANCELED);
    }
    return 0;
}

static inline void suspend_with_cancellation(pthread_t self) {
    sigset_t mask;
    sigjmp_buf jmpbuf;
    sigprocmask(SIG_SETMASK, NULL, &mask); // Get current signal mask
    sigdelset(&mask, PTHREAD_SIG_RESTART); // Unblock the restart signal
    // No need to save the signal mask, we'll restore it ourselves 
    /* Return 0 from siglongjump, non-0 from siglongjump.  
       Wake up on the restart signal, or in the cancel signal handler, return here via siglongJMP 
    */
    if (sigsetjmp(jmpbuf, 0) == 0) {
        self->p_cancel_jmp = &jmpbuf;
        // If it has been cancelled and is cancelable, it returns directly, otherwise hangs to wake up
        if (! (self->p_canceled && self->p_cancelstate == PTHREAD_CANCEL_ENABLE)) {
            do {
                sigsuspend(&mask);               // Wait for a signal
            } while (self->p_signal != PTHREAD_SIG_RESTART);
        }
        self->p_cancel_jmp = NULL;
    } else {
        // 从cancel信号的处理函数中的siglongjmp返回，重新设置信号掩码,屏蔽restart信号
        sigaddset(&mask, PTHREAD_SIG_RESTART); // Reblock the restart signal
        sigprocmask(SIG_SETMASK, &mask, NULL);
    }
}

// If the condition is met, the thread is awakened
int pthread_cond_signal(pthread_cond_t *cond) {
    pthread_t th;

    acquire(&cond->c_spinlock);
    // If the condition is met, the wake thread retrieves a blocked thread
    th = dequeue(&cond->c_waiting);
    release(&cond->c_spinlock);
    // Send a signal to wake it up
    if (th != NULL) {
        restart(th);
    }
    return 0;
}

static inline void restart(pthread_t th) {
    kill(th->p_pid, PTHREAD_SIG_RESTART);
}

// If the condition is met, all threads are awakened
int pthread_cond_broadcast(pthread_cond_t *cond) {
    pthread_queue tosignal;
    pthread_t th;

    acquire(&cond->c_spinlock);
    // Copy the current state of the waiting queue and empty it
    tosignal = cond->c_waiting;
    queue_init(&cond->c_waiting);
    release(&cond->c_spinlock);
    // Now signal each process in the queue
    while ((th = dequeue(&tosignal)) != NULL) {
        restart(th);
    }
    return 0;
}
