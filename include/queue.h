#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdbool.h>
#include "procinfo.h"

struct qnode {
	proc_info_t *proc;
	struct qnode *next;
};

typedef struct {
	struct qnode *first, *last;
} queue_t;

/* Initializes the queue as empty */
void init_queue(queue_t *q);

bool qisempty(queue_t *q);

/* Adds the given procinfo to the queue, 
   returns false on error. */
bool qpush(queue_t *q, proc_info_t *proc);

/* Removes and returns the first item in the queue,
   returns NULL on empty queue */
proc_info_t* qpop(queue_t *q);

#endif /* __QUEUE_H */
