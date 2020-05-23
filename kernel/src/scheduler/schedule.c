#include "scheduler/schedule.h"

#include "cpu/asm.h"
#include "os.h"
#include "memory/heap.h"
#include "memory/paging.h"

process_t *init;
process_t *current_process;
process_t *current_process_phys;

bool locked;

/*
 * This runs in machine mode. idk if that's a good idea, but it does.
 */

CREATE_MUTEX(LOCK_MUTEX)

void
set_proc(uint64_t addr)
{
	current_process = (process_t *) addr;
	current_process_phys = (process_t *) m_virt_to_phys(addr);
}

void
schedule()
{
	if(LOCK_MUTEX)
		return;

	if(init->next == 0) {
		current_process_phys = current_process;	
	} else {
		current_process_phys = (process_t *) m_virt_to_phys((uint64_t) current_process);
		if(current_process_phys->next == 0)
			set_proc((uint64_t) init);
		else
			set_proc((uint64_t) current_process_phys->next);
	}

	csr_write(mscratch, (uint64_t) &current_process_phys->frame);	
}
