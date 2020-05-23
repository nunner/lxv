#include "scheduler/proc.h"

#include "os.h"

#include "scheduler/schedule.h"
#include "memory/heap.h"
#include "memory/paging.h"
#include "string.h"

extern process_t *current_process;
extern process_t *init;
extern pt_t *root_table;

extern void main();
extern volatile void __kernel_proc;
extern volatile void __kernel_stack;

CREATE_MUTEX(PROC_MUTEX)

/*
 * This functions runs in M-Mode!
 */
void
setup_init_proc()
{
	init = (process_t *) &__kernel_proc;	
	current_process = init;

	init->state = RUNNING;
	init->frame.pc = (uint64_t) main;
	sp(init->frame) = (uint64_t) &__kernel_stack;
	init->pid = 0;
	init->next = 0;
}

void
start_kernel_process(job_t job)
{
	static uint64_t pid = 0;
	process_t *proc = malloc(sizeof(process_t));
	memset(proc, 0, sizeof(process_t));		

	proc->state = RUNNING;
	proc->frame.pc = (uint64_t) job;
	sp(proc->frame) = (uint64_t) &proc->stack[4093];
	//proc->frame.satp = (uint64_t) virt_to_phys((uint64_t) root_table);
	proc->pid = pid++;

	lock();
	process_t *idx = current_process;
	while(idx->next != 0)
		idx = idx->next;
	idx->next = proc;
	unlock();
}
