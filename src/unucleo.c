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

// Scheduler context
ucontext_t sched_ctx;



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
	
	stack = NULL;
	
	//Initializes the context of the Scheduler function
	getcontext(&sched_ctx);

	stack = malloc(STACK_SIZE);
	if (stack==NULL) return 2;
	sched_ctx.uc_stack.ss_sp = stack;
	sched_ctx.uc_stack.ss_size = STACK_SIZE;
	
	makecontext(&sched_ctx, scheduler, 0);

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
	dispatch();
}


/* Pauses execution, allowing other processes to execute. */
void mproc_yield(void)
{
	qpush(&process_priority[running_process->priority],running_process);
	swapcontext(&running_process->ctx,&sched_ctx);
}

/* Stops execution until the given process ends.
 * Returns 0 if successful (i.e. returned to
 * execution after end of other process). */
int mproc_join(int pid)
{
	proc_info_t* blocker = get_proc(pid);	
	qpush(&blocker->blocked,running_process);
	swapcontext(&running_process->ctx,&sched_ctx)
	return 0;
}

/* Starts the execution of processes and returns
 * control to the caller when there is none.
 * MUST ONLY BE CALLED ON MAIN.
 */
void scheduler()
{
	get_context(&sched_ctx);
	dispatcher();
	
	return;
}

void dispatcher()
{
	
//Select the next process based on priority
	if(!qisempty(&process_priority[HIGH]))
	{
		running_process = qpop(&process_priority[HIGH]);
		set_context(&running_process->ctx);
	}
	else if(!qisempty(&process_priority[MEDIUM]))
	{
		running_process = qpop(&process_priority[MEDIUM]);
		set_context(&running_process->ctx);
	}
	else if(!qisempty(&process_priority[LOW]))
	{
		running_process = qpop(&process_priority[LOW]);
		set_context(&running_process->ctx);
	}
//No more processes to be scheduled into the processor
	return;	
}
