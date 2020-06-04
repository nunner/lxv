#include "cpu/asm.h"

#include "cpu/machine/interrupt.h"
#include "cpu/machine/plic.h"
#include "cpu/supervisor/interrupt.h"

#include "memory/mmu.h"
#include "scheduler/schedule.h"

#include "driver/timer.h"

#define S_ECALL (1 << 9)

#define EEXCEPTIONS S_ECALL
#define IEXCEPTIONS 0

extern void 
machine_trap_entry();

extern void
setup_init_proc();

extern process_t *init;

void
setup_cpu()
{
	/*
	li t0, 0xffff

	csrrw x0, medeleg, t0
	csrrw x0, mideleg, t0

	li t0, 0xffffffff

	csrrw x0, mie, t0

	li t0, 0b11

	csrrw x0, mcounteren, t0
	*/

	setup_machine_interrupts();
	csr_set(medeleg, ~((uint64_t) 0) & ~(EEXCEPTIONS));
	csr_set(mideleg, ~((uint64_t) 0) & ~(IEXCEPTIONS));
	csr_write(mie, 0xffffffff);

	csr_read_set(mstatus, 1<<3);
	csr_write(mtvec, machine_trap_entry);

	setup_init_proc();
	csr_write(mscratch, &init->frame);

	init_plic();

	register_plic_interrupt(0x08);
	register_plic_interrupt(0x0a);

	set_limit(get_time() + FREQUENCY);
}
