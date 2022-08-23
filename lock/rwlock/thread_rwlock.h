#include <errno.h>
#include <stdio.h>

typedef struct {
    pthread_mutex_t   rw_mutex;
    pthread_cond_t    rw_condreaders;
    pthread_cond_t    rw_condwriters;
    int               rw_magic;
    int               rw_nwaitreaders;
    int               rw_nwaitwriters;
    int               rw_refcount;
} thread_rwlock_t;

#define RW_MAGIC 0X19283706
#define thread_rwlock_INITIALIZER { PTHREAD_MUTEX_INITIALIZER, \
    PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, \
    RW_MAGIC, 0, 0, 0 }

typedef int thread_rwlockattr_t; /*dummy; not supported*/

int thread_rwlock_init(thread_rwlock_t *rw, thread_rwlockattr_t *attr) {
    int     result;

    if (attr != NULL)
        return(EINVAL);     /* not supported */

    if ( (result = pthread_mutex_init(&rw->rw_mutex, NULL)) != 0)
        goto err1;
    if ( (result = pthread_cond_init(&rw->rw_condreaders, NULL)) != 0)
        goto err2;
    if ( (result = pthread_cond_init(&rw->rw_condwriters, NULL)) != 0)
        goto err3;

    rw->rw_nwaitreaders = 0;
    rw->rw_nwaitwriters = 0;
    rw->rw_refcount = 0;
    rw->rw_magic = RW_MAGIC;
    return(0);

err3:
    pthread_cond_destroy(&rw->rw_condreaders);
err2:
    pthread_mutex_destroy(&rw->rw_mutex);
err1:
    return(result);         /* an errno value */
}

int thread_rwlock_destroy(thread_rwlock_t *rw) {
    if (rw->rw_magic != RW_MAGIC)
        return(EINVAL);

    // destroy only when not in use 
    if (rw->rw_refcount != 0 ||
            rw->rw_nwaitreaders != 0 || rw->rw_nwaitwriters != 0)
        return(EBUSY);

    pthread_mutex_destroy(&rw->rw_mutex);
    pthread_cond_destroy(&rw->rw_condreaders);
    pthread_cond_destroy(&rw->rw_condwriters);
    rw->rw_magic = 0;

    return(0);
}

int thread_rwlock_rdlock(thread_rwlock_t *rw) {
    int     result;
    if (rw->rw_magic != RW_MAGIC)
        return(EINVAL);

    if ( (result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
        return(result);

    // a. check if writer lock b. check if some writers waiting
    while (rw->rw_refcount < 0 || rw->rw_nwaitwriters > 0) {
        rw->rw_nwaitreaders++;
        result = pthread_cond_wait(&rw->rw_condreaders, &rw->rw_mutex);
        rw->rw_nwaitreaders--;
        if (result != 0)
            break;
    }
    if (result == 0)
        rw->rw_refcount++;      /* another reader has a read lock */

    pthread_mutex_unlock(&rw->rw_mutex);
    return (result);
}

int thread_rwlock_tryrdlock(thread_rwlock_t *rw) {
    int     result;

    if (rw->rw_magic != RW_MAGIC)
        return(EINVAL);

    if ( (result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
        return(result);

    if (rw->rw_refcount < 0 || rw->rw_nwaitwriters > 0)
        result = EBUSY;         /* held by a writer or waiting writers */
    else
        rw->rw_refcount++;      /* increment count of reader locks */

    pthread_mutex_unlock(&rw->rw_mutex);
    return(result);
}

int thread_rwlock_wrlock(thread_rwlock_t *rw) {
    int     result;

    if (rw->rw_magic != RW_MAGIC)
        return(EINVAL);

    if ( (result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
        return(result);

    // a. current is writer lock, rw->rw_refcount = -1
    // b. current is reader lock, rw->rw_refcount > 0
    while (rw->rw_refcount != 0) {
        rw->rw_nwaitwriters++;
        result = pthread_cond_wait(&rw->rw_condwriters, &rw->rw_mutex);
        rw->rw_nwaitwriters--;
        if (result != 0)
            break;
    }
    if (result == 0)
        rw->rw_refcount = -1;

    pthread_mutex_unlock(&rw->rw_mutex);
    return(result);
}

int thread_rwlock_trywrlock(thread_rwlock_t *rw) {
    int     result;

    if (rw->rw_magic != RW_MAGIC)
        return(EINVAL);

    if ( (result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
        return(result);

    if (rw->rw_refcount != 0)
        result = EBUSY;         /* held by either writer or reader(s) */
    else
        rw->rw_refcount = -1;   /* available, indicate a writer has it */

    pthread_mutex_unlock(&rw->rw_mutex);
    return(result);
}

int thread_rwlock_unlock(thread_rwlock_t *rw) {
    int     result;

    if (rw->rw_magic != RW_MAGIC)
        return(EINVAL);

    if ( (result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
        return(result);

    if (rw->rw_refcount > 0)
        rw->rw_refcount--;          /* release current reader */
    else if (rw->rw_refcount == -1)
        rw->rw_refcount = 0;        /* release current writer */
    else
        fprintf(stderr, "rw_refcount = %d", rw->rw_refcount);

    if (rw->rw_nwaitwriters > 0) {
        // wake up a writer
        if (rw->rw_refcount == 0)
            result = pthread_cond_signal(&rw->rw_condwriters);
    } else if (rw->rw_nwaitreaders > 0) {
        // wake up all readers
        result = pthread_cond_broadcast(&rw->rw_condreaders);
    }

    pthread_mutex_unlock(&rw->rw_mutex);
    return(result);
}
