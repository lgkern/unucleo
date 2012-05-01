#include <stdbool.h>
#include <stdlib.h>
#include "procinfo.h"


/* Allocates a process and PID,
   return NULL if all are occupied */
proc_info_t *alloc_proc();

/* Frees the PID and memory for the process */
void  free_proc(proc_info_t *);

/* Array of all processes */
struct {
	bool used;
	proc_info_t proc;
} all_procs[MAX_PROCS] = { [0 ... MAX_PROCS-1].used = false };



/*** API Functions ***/
proc_info_t *create_proc(int prio, init_fn_t init_fn, void *arg,
						 ucontext_t *uc_link)
{
	/* Allocate a PID and proc_info */
	proc_info_t *proc = alloc_proc();
	if (proc == NULL) return NULL;

	/* Allocate stack space */
	void *sp = malloc(STACK_SIZE);
	if (sp == NULL) {
		free_proc(proc);
		return NULL;
	}

	/* Initialize blocked list and priority */
	init_queue(&proc->blocked);
	proc->priority = prio;

	/* Initialize the context with stack and uc_link */
	getcontext(&proc->ctx);
	proc->ctx.uc_stack.ss_sp   = sp;
	proc->ctx.uc_stack.ss_size = STACK_SIZE;
	proc->ctx.uc_link = uc_link;

	/* Initialize the context with its function */
	makecontext(&proc->ctx, (void(*)())init_fn, 1, arg);

	return proc;
}

proc_info_t *get_proc(pid_t pid)
{
	if (pid < 0 || MAX_PROCS <= pid || !all_procs[pid].used) {
		return NULL;
	} else {
		return &all_procs[pid].proc;
	}
}

void destroy_proc(proc_info_t *proc)
{
	/* Free the stack that was allocated to this process */
	free(proc->ctx.uc_stack.ss_sp);

	/* Clear the queue of blocked processes (should already be empty, though) */
	while (!qisempty(&proc->blocked)) {
		qpop(&proc->blocked);
	}

	/* Free the process and PID */
	free_proc(proc);
}

void invoke_proc(proc_info_t *proc, ucontext_t *out_context)
{
	swapcontext(out_context, &proc->ctx);
}



/*** PID and memory management ***/
pid_t first_free=0; /* The first free PID or -1 */

proc_info_t *alloc_proc()
{
	/* If all PIDs are used, return NULL */
	if (first_free < 0) {
		return NULL;
	}

	/* Update the first free PID */
	pid_t pid=first_free++;
	while (all_procs[first_free].used && first_free < MAX_PROCS) {
		first_free++;
	}
	if (MAX_PROCS <= first_free) {
		first_free = -1;
	}

	/* Mark the PID as used and return its proc */
	all_procs[pid].used = true;
	all_procs[pid].proc.pid = pid;
	return &all_procs[pid].proc;
}

void free_proc(proc_info_t *proc)
{
	/* Mark the PID as unused */
	all_procs[proc->pid].used = false;

	/* Update the first free PID */
	if (proc->pid < first_free) {
		first_free = proc->pid;
	}
}
