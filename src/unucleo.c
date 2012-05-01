#include <stdlib.h>
#include <ucontext.h>

#include "procinfo.h"
#include "queue.h"
#include "unucleo.h"


/*** Internal API ***/

/* Handler for the end of process:
 *  unblocks waiting processes,
 *  delegates to the scheduler.
 */
void return_handler();

/* Selects a process from the ready
 * queues and jumps to its context */
void dispatch();




// Default process queues
queue_t process_priority[TOTAL_PRIORITIES];

// Structure of the current running process
proc_info_t *running_process;

// Context to which all processes return
ucontext_t proc_end_ctx;



/* Initializes the ukernel.
 * Returns 0 on success. */
int libsisop_init()
{
	//Initializes process queues
	init_queue(&process_priority[LOW]);
	init_queue(&process_priority[MEDIUM]);
	init_queue(&process_priority[HIGH]);

	//Initializes the current process structure
	running_process=NULL;

	//Initializes the context for the end of each process
	getcontext(&proc_end_ctx);

	void *stack = malloc(STACK_SIZE);
	if (stack==NULL) return 1;
	proc_end_ctx.uc_stack.ss_sp = stack;
	proc_end_ctx.uc_stack.ss_size = STACK_SIZE;

	makecontext(&proc_end_ctx, return_handler, 0);

	return 0;
}

/* Creates a new process with given priority,
 * which will execute start_routine with arg
 * until it returns.
 * Returns 0 on success. */
int mproc_create(int prio, void *(*start_routine)(void*), void *arg)
{
	return 0;
}

/* Pauses execution, allowing other processes to execute. */
void mproc_yield(void)
{

}

/* Stops execution until the given process ends.
 * Returns 0 if successful (i.e. returned to
 * execution after end of other process). */
int mproc_join(int pid)
{
	return 0;
}

/* Starts the execution of processes and returns
 * control to the caller when there is none.
 * MUST ONLY BE CALLED ON MAIN.
 */
void scheduler()
{

}
