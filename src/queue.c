#include <stdlib.h>
#include "queue.h"

void init_queue(queue_t *q) {
	/* Initialize the queue as empty */
	q->first = q->last = NULL;
}

bool qisempty(queue_t *q) {
	return q->first == NULL;
}

bool qpush(queue_t *q, proc_info_t *proc)
{
	/* Creates the queue node (ensure it is allocated) */
	struct qnode *node = (struct qnode*) malloc(sizeof(struct qnode));
	if (node == NULL) return false;

	/* Attach the node to the queue's list */
	node->proc = proc;
	node->next = q->last;

	/* Update the queue's list */
	q->last = node;
	if (q->first == NULL) q->first = node;

	return true;
}

proc_info_t* qpop(queue_t *q)
{
	/* Keep references to avoid memory leaks */
	struct qnode *popped = q->first;
	proc_info_t *proc = popped->proc;

	/* Do nothing if the queue is empty */
	if (q->first == NULL) return NULL;

	/* Update the queue's list */
	if (q->first == q->last) q->last = q->first->next;
	q->first = q->first->next;

	/* Clean up */
	free(popped);
	return proc;
}
