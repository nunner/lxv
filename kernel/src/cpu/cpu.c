#include "cpu/asm.h"

#include "cpu/machine/interrupt.h"
#include "cpu/supervisor/interrupt.h"

#include "driver/timer.h"

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

	csr_write(medeleg, 0xffffffff);
	csr_write(mideleg, 0xffffffff);
	csr_write(mie, 0xffffffff);

	csr_read_set(mstatus, 1<<3);
	csr_write(mtvec, handle_machine_trap);

	set_limit(get_time() + FREQUENCY);
}
