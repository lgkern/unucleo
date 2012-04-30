#include <stdlib.h>
#include "procinfo.h"

typedef void (*ucontext_fn_t)(void);
pid_t free_pid=0;

proc_info_t *create_proc(init_fn_t init_fn, void *arg,
						 ucontext_t *uc_link, 
						 int block_idx)
{
	/* Allocate space for the process structure */
	proc_info_t *proc = (proc_info_t *) malloc(sizeof(proc_info_t));
	if (proc == NULL) return NULL;

	/* Allocate stack space */
	void *sp = malloc(STACK_SIZE);
	if (sp == NULL) {
		free(proc);
		return NULL;
	}
	
	/* Initialize PID and blocked list index */
	proc->pid = free_pid++;
	proc->block_idx = block_idx;

	/* Initialize the context with stack and uc_link */
	getcontext(&proc->ctx);
	proc->ctx.uc_stack.ss_sp   = sp;
	proc->ctx.uc_stack.ss_size = STACK_SIZE;
	proc->ctx.uc_link = uc_link;

	/* Initialize the context with its function */
	makecontext(&proc->ctx, (ucontext_fn_t)init_fn, 1, (int)arg);
	
	return proc;
}


void destroy_proc(proc_info_t *proc)
{
	/* Free the stack that was allocated to this process */
	free(proc->ctx.uc_stack.ss_sp);

	/* Free the process itself */
	free(proc);
}

void invoke_proc(proc_info_t *proc, ucontext_t *out_context)
{
	swapcontext(out_context, &proc->ctx);
}
