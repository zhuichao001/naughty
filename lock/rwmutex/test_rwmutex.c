#include "rwmutex.h"

// test codes
// 0 : regular mutex
// 1 : pthread_rwlock
// 2 : rwmutex

#define RW_LOCK  2

#if RW_LOCK == 0
  static struct rwmutex rw;
  #define RW_INIT(rw)    rwmutex_init(rw)
  #define RW_RLOCK(rw)   pthread_mutex_lock(&(rw)->mutex)
  #define RW_WLOCK(rw)   pthread_mutex_lock(&(rw)->mutex)
  #define RW_UNLOCK(rw)  pthread_mutex_unlock(&(rw)->mutex);
#elif RW_LOCK == 1
  static pthread_rwlock_t rw;
#define RW_INIT(rw)    pthread_rwlock_init(rw, NULL)
  #define RW_RLOCK(rw)   pthread_rwlock_rdlock(rw)
  #define RW_WLOCK(rw)   pthread_rwlock_wrlock(rw)
  #define RW_UNLOCK(rw)  pthread_rwlock_unlock(rw)
#else
  static struct rwmutex rw;
  #define RW_INIT(rw)    rwmutex_init(rw)
  #define RW_RLOCK(rw)   rwmutex_rlock(rw)
  #define RW_WLOCK(rw)   rwmutex_wlock(rw)
  #define RW_UNLOCK(rw)  rwmutex_unlock(rw)
#endif

// sleep from 0 to 200us
#define rw_delay()   usleep(random()%100)

#define val_orig    100000
static unsigned gval = val_orig;

static void * thread_reader(void *arg) {
	int *pc = (int*)arg;

	while(1) {
		RW_RLOCK(&rw);
		if (gval == 0) {
		    RW_UNLOCK(&rw);
			break;
		}

		*pc += 1;
		RW_UNLOCK(&rw);

		rw_delay();
	}

	return NULL;
}

static void * thread_writer(void *arg) {
	int *pc = (int*)arg;

	while(1) {
		RW_WLOCK(&rw);
		if (gval==0){
		    RW_UNLOCK(&rw);
			break;
        } else {
			--gval;
		    *pc += 1;
		}

		RW_UNLOCK(&rw);

		rw_delay();
	}

	return NULL;
}

void test(){
	RW_INIT(&rw);

    const int NUM_READER = 30;
    const int NUM_WRITER = 5;

    pthread_t readers[NUM_READER];
    pthread_t writers[NUM_WRITER];

    int winfo[NUM_WRITER];
    int rinfo[NUM_READER];
    memset(winfo, 0, sizeof(winfo)*sizeof(int));
    memset(rinfo, 0, sizeof(rinfo)*sizeof(int));

	for(int i=0; i<NUM_READER; i++) {
		pthread_create(&readers[i], NULL, thread_reader, (void *)(&rinfo[i]));
	}

	for(int i=0; i<NUM_WRITER; i++) {
		pthread_create(&writers[i], NULL, thread_writer, (void *)(&winfo[i]));
	}

	for(int i=0; i< NUM_WRITER; i++) {
		pthread_join(writers[i], NULL);
		fprintf(stderr, "writer[%d] count: %d\n", i, winfo[i]);
	}

	for(int i=0; i< NUM_READER; i++) {
		pthread_join(readers[i], NULL);
		fprintf(stderr, "reader[%d] : %d\n", i, rinfo[i]);
	}
}

int main(void) {
    test();
	return 0;
}
