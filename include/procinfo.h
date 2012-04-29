#ifndef __PROCINFO_H
#define __PROCINFO_H

#include <stdio.h>
#include <ucontext.h>

#define STACK_SIZE 16384

typedef int pid_t;
typedef void *(*init_fn_t)(void *);

typedef struct {
	pid_t pid;       /* Unique PID (never repeated) */
	ucontext_t ctx;  /* Context for its execution */
	int block_idx;   /* Index for its list of blocked processes */	
} proc_info_t;


/* Creates a process and allocates its stack */
proc_info_t *create_proc(init_fn_t init_fn, void *arg,
						 ucontext_t *uc_link, 
						 int block_idx);

/* Destroys a process and its stack */
void destroy_proc(proc_info_t *proc);

/* Invokes the process and saves the current context */
void invoke_proc(proc_info_t *proc, ucontext_t *out_context);

#endif /* __PROCINFO_H */
