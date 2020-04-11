#include "cpu/cpu.h"
#include "cpu/asm.h"
#include "cpu/machine/interrupt.h"
#include "cpu/machine/plic.h"

#include "driver/uart.h"

#define INTERRUPT 0x8000000000000000
#define CODE ~(INTERRUPT)

#define enable_machine_interrupt(x) (csr_set(mie, 1<<x))

void
setup_machine_interrupts() {
	init_plic();
	enable_machine_interrupts();

	// Enable internal interrupts.
	enable_machine_interrupt(11);

	// Enable interrupts on the PLIC controller
	register_plic_interrupt(10);
}

void
enable_machine_interrupts()
{
	csr_write(mtvec, handle_machine_trap);
	csr_read_set(mstatus, 1<<3);
}

void
handle_machine_trap()
{
	uint64_t interrupt 	= csr_read(mcause) & INTERRUPT;
	uint64_t code 		= csr_read(mcause) & CODE;

	(void) code;

	if(interrupt) {
		uart_write("Interrupt.\n");
	} else {
		uart_write("Exception.\n");
	}
}
