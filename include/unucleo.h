#ifndef __UNUCLEO_H
#define __UNUCLEO_H

#define TOTAL_PRIORITIES 3
#define HIGH 0
#define MEDIUM 1
#define LOW 2

/* Initializes the ukernel.
 * Returns 0 on success. */
int libsisop_init();

/* Creates a new process with given priority,
 * which will execute start_routine with arg
 * until it returns.
 * Returns the PID of the created process or -1 */
int mproc_create(int prio, void *(*start_routine)(void*), void *arg);

/* Pauses execution, allowing other processes to execute. */
int mproc_yield(void);

/* Stops execution until the given process ends.
 * Returns 0 if successful (i.e. returned to
 * execution after end of other process). */
int mproc_join(int pid);

/* Starts the execution of processes and returns
 * control to the caller when there is none.
 * MUST ONLY BE CALLED ON MAIN.
 */
void scheduler();

#endif /* __UNUCLEO_H */
