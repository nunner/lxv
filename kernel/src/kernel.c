#include "os.h"

#include "cpu/supervisor/interrupt.h"
#include "driver/uart.h"
#include "driver/virtio.h"
#include "memory/mmu.h"
#include "memory/paging.h"

void 
main()
{
	// Setup the UART
	setup_uart();

	kprintf("Entering the kernel task.\n");

	enable_supervisor_interrupts();

	// Enable Sv39 paging
	init_paging();
	setup_heap();

	scan_virtio();

	for(;;);
}
