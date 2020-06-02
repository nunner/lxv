#include "os.h"

#include "cpu/supervisor/interrupt.h"
#include "driver/uart.h"
#include "driver/virtio.h"
#include "memory/mmu.h"
#include "memory/paging.h"
#include "scheduler/proc.h"

// To clean the current process list. This has to run in S mode.
extern void clean_proc();

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

	start_kernel_process(clean_proc);

	for(;;);
}

void
panic(char *s)
{
	lock();
	kprintf("Kernel panic: %s", s);
	for(;;);
}
