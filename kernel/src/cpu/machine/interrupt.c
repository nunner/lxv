#include "cpu/machine/interrupt.h"

#include "os.h"

#include "scheduler/schedule.h"
#include "cpu/asm.h"
#include "driver/timer.h"
#include "stdint.h"

extern process_t *current_process;

// we are *only* going to handle the timer here, and then jump to the 
// supervisor handler.
void 
handle_machine_trap(uint64_t sp) 
{
	frame_t *frame = (frame_t *) csr_read(mscratch);
	frame->sp = sp;
	frame->pc = csr_read(mepc);

	set_limit(get_time() + FREQUENCY);

	schedule();
	frame = (frame_t *) csr_read(mscratch);

	csr_write(mepc, frame->pc);
}
