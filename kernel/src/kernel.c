#include "os.h"

#include "cpu/cpu.h"
#include "cpu/asm.h"
#include "cpu/supervisor/interrupt.h"
#include "driver/uart.h"

int i[32];

void 
main()
{
	// Setup the UART
	setup_uart();
	enable_supervisor_interrupts();
	// Enable Sv39 paging
	csr_read_set(satp, (uint64_t) 8 << 60);

	uart_write("Hello, world\n");

	while(1)
		__asm__("wfi");
}
