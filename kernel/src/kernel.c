#include "os.h"

#include "cpu/supervisor/interrupt.h"
#include "driver/uart.h"
#include "memory/mmu.h"
#include "memory/paging.h"


void 
main()
{
	// Setup the UART
	setup_uart();
	enable_supervisor_interrupts();

	// Enable Sv39 paging
	init_paging();
	setup_heap();

	node_t *test = malloc(PAGE_SIZE);
	free(test);

	uart_write("Hello, world\n");

	while(1)
		__asm__("wfi");
}
