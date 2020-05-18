#include "cpu/machine/interrupt.h"

#include "os.h"

#include "scheduler/schedule.h"
#include "cpu/asm.h"
#include "driver/timer.h"
#include "stdint.h"

extern uint64_t handler_ret;

// we are *only* going to handle the timer here, and then jump to the 
// supervisor handler.
void __attribute__((interrupt("machine")))
handle_machine_trap() 
{
	set_limit(get_time() + FREQUENCY);
	csr_write(mepc, schedule);
}
