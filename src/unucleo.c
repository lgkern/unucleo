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
char proc_end_stack[STACK_SIZE];

// Context of the scheduler
ucontext_t sched_ctx;

// Main context from which the scheduler was first called
ucontext_t main_ctx;



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
	proc_end_ctx.uc_stack.ss_sp = proc_end_stack;
	proc_end_ctx.uc_stack.ss_size = sizeof(proc_end_stack);

	makecontext(&proc_end_ctx, return_handler, 0);

	return 0;
}


/* Creates a new process with given priority,
 * which will execute start_routine with arg
 * until it returns.
 * Returns the PID of the created process or -1 */
int mproc_create(int prio, void *(*start_routine)(void*), void *arg)
{
	/* Verifies the priority */
	if (prio != MEDIUM && prio != LOW) {
		return -1;
	}

	/* Creates the process itself */
	proc_info_t *proc = create_proc(prio, start_routine, arg, &proc_end_ctx);
	if (proc == NULL) {
		return -1;
	}

	/* Adds the process to its ready queue */
	if (!qpush(&process_priority[proc->priority], proc)) {
		destroy_proc(proc);
		return -1;
	}

	return proc->pid;
}

void return_handler()
{
	/* Unblocks waiting processes */
	while (!qisempty(&running_process->blocked)) {
		proc_info_t *unblocked = qpop(&running_process->blocked);
		qpush(&process_priority[unblocked->priority], unblocked);
	}

	destroy_proc(running_process);
	setcontext(&sched_ctx);
}


/* Pauses execution, allowing other processes to execute. */
int mproc_yield(void)
{
	/* Puts the current process back into the ready queues */
	if ( !qpush(&process_priority[running_process->priority], running_process) ) {
		return 1;
	}

	/* Switches to the scheduler */
	swapcontext(&running_process->ctx, &sched_ctx);
	return 0;
}

/* Stops execution until the given process ends.
 * Returns 0 if successful (i.e. returned to
 * execution after end of other process). */
int mproc_join(int pid)
{
	/* Puts the process into the blocked queue */
	proc_info_t* blocker = get_proc(pid);
	if (!qpush(&blocker->blocked,running_process)) {
		return 1;
	}

	/* Switches to the scheduler */
	swapcontext(&running_process->ctx, &sched_ctx);
	return 0;
}

/* Starts the execution of processes and returns
 * control to the caller when there is none.
 * MUST ONLY BE CALLED ON MAIN.
 */
void scheduler()
{
	/* Prepare the context for the scheduler */
	getcontext(&sched_ctx);

	char stack[STACK_SIZE];
	sched_ctx.uc_stack.ss_sp   = stack;
	sched_ctx.uc_stack.ss_size = STACK_SIZE;
	sched_ctx.uc_link = &main_ctx;

	makecontext(&sched_ctx, dispatch, 0);

	/* Change context */
	swapcontext(&main_ctx, &sched_ctx);
}

void dispatch()
{
	//Select the next process based on priority
	if(!qisempty(&process_priority[HIGH]))
	{
		running_process = qpop(&process_priority[HIGH]);
		setcontext(&running_process->ctx);
	}
	else if(!qisempty(&process_priority[MEDIUM]))
	{
		running_process = qpop(&process_priority[MEDIUM]);
		setcontext(&running_process->ctx);
	}
	else if(!qisempty(&process_priority[LOW]))
	{
		running_process = qpop(&process_priority[LOW]);
		setcontext(&running_process->ctx);
	}

	/* No more processes to be scheduled into the processor,
	   we should go back to the main context */
	setcontext(&main_ctx);
}
