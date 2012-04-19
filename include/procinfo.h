#ifndef __PROCINFO_H
#define __PROCINFO_H

struct proc_info;
typedef struct proc_info proc_info_t;

proc_info_t *create_proc(/* ... */);
void destroy_proc(proc_info_t *proc);
void invoke_proc(proc_info_t *proc);

#endif /* __PROCINFO_H */
