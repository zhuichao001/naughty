#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <stdint.h>
#include <memory.h>

const int RW_READER = 0;
const int RW_WRITER = 1;

typedef enum{ false , true }bool;

//#define RW_DEBUG

#ifdef RW_DEBUG
#define rwlog(...)				\
	printf("[ %x ]", (int)pthread_self());	\
	printf(__VA_ARGS__)
#else
#define rwlog(...)
#endif

#define LIST_INIT(head)				\
	(head)->next = (head);			\
	(head)->prev = (head)

#define LIST_INSERT(prv, cur)		\
	(cur)->next = (prv)->next;		\
	(cur)->prev = (prv);			\
	(prv)->next->prev = (cur);		\
	(prv)->next = (cur)

#define LIST_APPEND(head, cur)		\
	(cur)->prev = (head)->prev;		\
	(cur)->next = (head);			\
	(head)->prev->next = (cur);		\
	(head)->prev = (cur)

#define LIST_REMOVE(cur)			\
	(cur)->prev->next = (cur)->next;	\
	(cur)->next->prev = (cur)->prev

#define LIST_EMPTY(head)	((head)->next == (head))


struct rwnode {
	int type; // -1: None, 0: Reader, 1: Writer
	int num;
	unsigned seq;
	struct rwnode * prev;
	struct rwnode * next;
};

struct rwmutex {
	unsigned seq;
	struct rwnode head;
	pthread_mutex_t mutex;
	pthread_cond_t conds[2];  // use a cond pool to avoid thundering herd
};

static void rwnode_dump(struct rwnode * item) {
	rwlog("\trwnode: %p {.prev = %p, .next=%p, .type=%d, .num=%d }\n",
	      item, item->prev, item->next, item->type, item->num);
}

static void rwmutex_dump(struct rwmutex* rw) {
	struct rwnode * item = rw->head.next;
	rwlog("\trwmutex: %p {\n", rw);
	while(item != &rw->head){
		rwnode_dump(item);
		item = item->next;
	}
	rwlog("\t}\n");
}

bool rwlist_empty(struct rwmutex *rw){
	return LIST_EMPTY(&rw->head) && rw->head.num == 0;
}

bool rwlist_single_reader(struct rwmutex *rw) {
	return LIST_EMPTY(&rw->head) && rw->head.type == RW_READER && rw->head.num > 0;
}

int rwmutex_init(struct rwmutex * rw) {
	LIST_INIT(&rw->head);
	if (pthread_mutex_init(&rw->mutex, NULL)<0) {
		return -1;
	}

	if (pthread_cond_init(&rw->conds[0], NULL)<0) {
		return -1;
	}

	if (pthread_cond_init(&rw->conds[1], NULL)<0) {
		return -1;
	}

	rw->seq = 0;
	rw->head.type = -1;
	rw->head.seq = 0;
	rw->head.num = 0;
	return 0;
}

int rwmutex_rlock(struct rwmutex * rw) {
	struct rwnode item = {
		.type = RW_READER,
	};

	pthread_mutex_lock(&rw->mutex);

	struct rwnode *reader = NULL;
	struct rwnode *tail = rw->head.prev;

	if (rwlist_empty(rw)) { // occupied
		reader = NULL;
		rw->head.num = 1;
		rw->head.type = RW_READER;
		rw->head.seq = rw->seq;
	} else if (rwlist_single_reader(rw)) { // shared
		reader = NULL;
		rw->head.num++;
		rw->head.type = RW_READER;
	} else if (!rwlist_empty(rw) && (tail->type == 0)) { //merged to last reader
		assert(tail->num > 0);
		reader = tail;
		reader->num++;
		rwlog("\t rlock merge into tail %p : ", tail);
		rwnode_dump(tail);
	} else { // new reader
		reader = &item;
		reader->num = 1;
		reader->seq = rw->seq;
		LIST_APPEND(&rw->head, reader);
		rwlog("\t rlock insert reader: %p\n", reader);
	}

	rw->seq++;

	if (reader) {
		unsigned seq = reader->seq;
		while(seq != rw->head.seq) {
			// wait for writer to wake up reader
			pthread_cond_wait(&rw->conds[RW_READER], &rw->mutex);
		}
	}

	pthread_mutex_unlock(&rw->mutex);
	return 0;
}

int rwmutex_wlock(struct rwmutex * rw) {
	struct rwnode item = {
		.num  = 1,
		.type = RW_WRITER,
	};

	pthread_mutex_lock(&rw->mutex);

	struct rwnode *writer = NULL; 
    struct rwnode *prev = rw->head.prev;

	if (rwlist_empty(rw)) {
		rw->head.num = 1;
		rw->head.type = RW_WRITER;
		rw->head.seq = rw->seq;
	} else {
		writer = &item;
		writer->seq = rw->seq;
		LIST_APPEND(&rw->head, writer); //insert to tail
	}

	++rw->seq;

	if (writer) {
		while(writer->seq != rw->head.seq) {
			// wait for reader to wakeup writer
			pthread_cond_wait(&rw->conds[RW_WRITER], &rw->mutex);
		}
	}

	pthread_mutex_unlock(&rw->mutex);
	return 0;
}

int rwmutex_unlock(struct rwmutex * rw) {
	pthread_mutex_lock(&rw->mutex);

	struct rwnode * head = &rw->head;

	if (head->type == RW_WRITER) {
		assert(head->num == 1);
	}

	if (--head->num == 0) {
		struct rwnode * next = head->next;
		if (next != head) {
			// wakeup next
			assert(next->num > 0);
			assert(next->type == RW_READER || next->type == RW_WRITER);

			head->type = next->type;
			head->seq  = next->seq;
			head->num  = next->num;

			LIST_REMOVE(next);

			if (next->type == RW_READER) { // next is reader, so wakup reader.
				pthread_cond_broadcast(&rw->conds[RW_READER]);
			} else { // next is writer, so wakup writer
				pthread_cond_broadcast(&rw->conds[RW_WRITER]);
			}
		} else {
			head->type = -1;
			head->seq  = 0;
			head->num  = 0;
			assert(head->prev == head && head->next == head);
		}
	}

	pthread_mutex_unlock(&rw->mutex);
	return 0;
}

