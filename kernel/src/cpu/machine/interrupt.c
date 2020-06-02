#include "cpu/machine/interrupt.h"

#include "os.h"

#include "cpu/asm.h"
#include "cpu/common.h"
#include "cpu/machine/plic.h"
#include "driver/timer.h"
#include "scheduler/schedule.h"
#include "stdint.h"

#define TIMER 		7
#define SYSCALL 	9
#define PLIC		11

extern process_t *current_process_phys;
extern process_t *init;
extern volatile void __machine_stack;

uint64_t machine_stack_pos = (uint64_t) &__machine_stack;

static void (*interrupts[32])(uint64_t value);
static void (*exceptions[32])(uint64_t value);

void
jump_to_s_mode(uint64_t func)
{
	csr_write(sscratch, csr_read(mepc));
	csr_write(mepc, func);
	__asm__("mv %0, sp" : "=r" (machine_stack_pos));
}

void 
machine_stub(uint64_t val)
{
	(void) val;
	frame_t *frame = (frame_t *) csr_read(mscratch);
	frame->pc = csr_read(mepc);
}

void
handle_timer(uint64_t val) 
{
	(void) val;

	frame_t *frame = (frame_t *) csr_read(mscratch);
	frame->pc = csr_read(mepc);

	set_limit(get_time() + FREQUENCY);

	schedule();
	frame = (frame_t *) csr_read(mscratch);

	__asm__("mv %0, sp" : "=r" (machine_stack_pos));
	csr_write(mepc, frame->pc);
}

void
handle_syscall(uint64_t num)
{
	frame_t *frame = (frame_t *) csr_read(mscratch);
	frame->pc = csr_read(mepc) + 4;

	switch(num) {
		case 0: 
				current_process_phys->state = STOPPED;	
				// This is supposed to fall through.
		case 1: {
					schedule();
					frame = (frame_t *) csr_read(mscratch);

					__asm__("mv %0, sp" : "=r" (machine_stack_pos));
				}
				break;
	}

	csr_write(mepc, frame->pc);
}

void
setup_machine_interrupts() 
{
	// Iinitialize your exceptions/interrupts here.
	REGISTER_INTERRUPT(TIMER, handle_timer);
	REGISTER_INTERRUPT(PLIC, handle_plic);
	REGISTER_EXCEPTION(SYSCALL, handle_syscall);
}

void 
handle_machine_trap() 
{
	uint64_t interrupt  = csr_read(mcause) & INTERRUPT;
	uint64_t code       = csr_read(mcause) & CODE;
	uint64_t val        = csr_read(mtval);

	switch(code) {
		case SYSCALL:
			__asm__("mv %0, a0": "=r" (val):);
			break;
	}

	if(interrupt) {
		if(interrupts[code]) interrupts[code](val);
		else machine_stub(val);
	} else {
		if(exceptions[code]) exceptions[code](val);
		else machine_stub(val);
	}
}
