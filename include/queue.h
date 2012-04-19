#ifndef __QUEUE_H
#define __QUEUE_H

#include "procinfo.h"

struct qnode {
	proc_info_t *proc;
	struct qnode *next;
};

typedef struct {
	struct qnode *first, *last;
} queue_t;

void init_queue(queue_t *q);

void qisempty(queue_t *q);
void qpush(queue_t *q, proc_info_t *proc);
proc_info_t* qpop(queue_t *q);

#endif /* __QUEUE_H */
