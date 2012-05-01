#ifndef __PROCINFO_H
#define __PROCINFO_H

#include <stdio.h>
#include <ucontext.h>
#include "queue.h"



#define STACK_SIZE 16384
#define MAX_PROCS  128



typedef int pid_t;
typedef void *(*init_fn_t)(void *);

struct proc_info {
    pid_t pid;        /* Unique PID (may be reused after end of process) */
    ucontext_t ctx;   /* Context for its execution */
    queue_t blocked;  /* List of processes blocked by this */
}; /* typedef'd to proc_info_t on queue.h */


/* Creates a process and allocates its stack */
proc_info_t *create_proc(init_fn_t init_fn, void *arg,
						 ucontext_t *uc_link,
						 int block_idx);

/* Obtains a process given its pid */
proc_info_t *get_proc(pid_t pid);

/* Destroys a process and its stack */
void destroy_proc(proc_info_t *proc);

/* Invokes the process and saves the current context */
void invoke_proc(proc_info_t *proc, ucontext_t *out_context);

#endif /* __PROCINFO_H */
