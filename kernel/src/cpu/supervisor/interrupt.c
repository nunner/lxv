#include "cpu/cpu.h"
#include "cpu/asm.h"
#include "cpu/supervisor/interrupt.h"
#include "cpu/supervisor/plic.h"

#include "driver/uart.h"

#define INTERRUPT 0x8000000000000000
#define CODE ~(INTERRUPT)

#define REGISTER_INTERRUPT(interrupt, handler) {\
	csr_set(sie, 1<<interrupt); \
	interrupts[interrupt] = handler; \
}

#define REGISTER_EXCEPTION(exception, handler) {\
	csr_set(sie, 1<<exception); \
	exceptions[exception] = handler; \
}

void stub(uint64_t i);

static void (*interrupts[32])(uint64_t value);
static void (*exceptions[32])(uint64_t value);

void
setup_supervisor_interrupts() {
	init_plic();

	// Iinitialize your exceptions/interrupts here.

	// Enable interrupts on the PLIC controller
	register_plic_interrupt(10);
}

void
enable_supervisor_interrupts()
{
	setup_supervisor_interrupts();
	csr_write(stvec, handle_supervisor_trap);
	csr_read_set(sstatus, 1<<3);
}

void
handle_supervisor_trap()
{
	uint64_t interrupt  = csr_read(scause) & INTERRUPT;
	uint64_t code       = csr_read(scause) & CODE;
	uint64_t val        = csr_read(stval);

	if(interrupt) {
		if(interrupts[code]) interrupts[code](val);
		else stub(val);
	} else {
		if(exceptions[code]) exceptions[code](val);
		else stub(val);
	}
}

void
stub(uint64_t value)
{
	(void) value;
	uart_write("Oida");
}

