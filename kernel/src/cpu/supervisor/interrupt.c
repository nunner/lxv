#include "cpu/supervisor/interrupt.h"

#include "scheduler/schedule.h"

#include "os.h"

#include "cpu/asm.h"
#include "cpu/common.h"
#include "cpu/cpu.h"
#include "driver/uart.h"


void stub(uint64_t code, uint64_t value);

static void (*interrupts[32])(uint64_t value);
static void (*exceptions[32])(uint64_t value);

void
setup_supervisor_interrupts() 
{
	// Iinitialize your exceptions/interrupts here.
	REGISTER_INTERRUPT(7, schedule);
}

void
enable_supervisor_interrupts()
{
	setup_supervisor_interrupts();
	csr_write(stvec, handle_supervisor_trap);
	csr_read_set(sstatus, 1<<3);
}

void __attribute__((interrupt("supervisor")))
handle_supervisor_trap() {
	uint64_t interrupt  = csr_read(scause) & INTERRUPT;
	uint64_t code       = csr_read(scause) & CODE;
	uint64_t val        = csr_read(stval);

	if(interrupt) {
		if(interrupts[code]) interrupts[code](val);
		else stub(code, val);
	} else {
		if(exceptions[code]) exceptions[code](val);
		else stub(code, val);
	}
}

void
stub(uint64_t code, uint64_t value)
{
	(void) value;
	kprintf("Got an interrupt/exception with code %d\n", code);
	csr_write(sepc, csr_read(sepc)+4);
}
