#include "cpu/cpu.h"
#include "cpu/asm.h"

#include "cpu/supervisor/interrupt.h"

#include "driver/uart.h"

#include "memory/heap.h"
#include "memory/mmu.h"
#include "memory/paging.h"

void 
main()
{
	// Setup the UART
	setup_uart();
	setup_heap();
	enable_supervisor_interrupts();

	// Enable Sv39 paging
	init_paging();

	uart_write("Hello, world\n");

	while(1)
		__asm__("wfi");
}
