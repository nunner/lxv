#include "cpu/cpu.h"
#include "cpu/interrupt.h"
#include "cpu/plic.h"

#include "driver/uart.h"

void
setup_interrupts()
{
	init_plic();
	enable_interrupts();

	enable_machine_interrupt(MACHINE_EXTERNAL);

	register_interrupt(10);
}

void
default_handler(uint64_t code)
{
	if(code == MACHINE_EXTERNAL) {
		uart_write("Interrupt!\n");
	} else {
		
	}
	uart_write("Oida!\n");
}
