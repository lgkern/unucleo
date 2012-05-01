#include <stdlib.h>
#include "unucleo.h"
#include "queue.h"
#include "procinfo.h"


//Default process queues
queue_t process_priority[TOTAL_PRIORITIES];

//Structure of the current running process
proc_info_t running_process;

/* Initializes the ukernel.
 * Returns 0 on success. */
int libsisop_init()
{
	//Initializes process queues
	init_queue(&process_priority[LOW]);
	init_queue(&process_priority[MEDIUM]);
	init_queue(&process_priority[HIGH]);
	
	//Initializes the current process structure
	running_process.pid = -1;
	init_queue(&running_process.blocked);
	
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
