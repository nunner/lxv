#include "os.h"

#include "cpu/supervisor/interrupt.h"
#include "driver/uart.h"
#include "driver/virtio.h"
#include "memory/mmu.h"
#include "memory/paging.h"
#include "scheduler/proc.h"

void
print()
{
	for(;;)
		kprintf("Task 2\n");
}

void
print2()
{
	for(;;)
		kprintf("Task 3\n");
}

void
print3()
{
	for(;;)
		kprintf("Task 4\n");
}

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

	start_kernel_process(print);
	start_kernel_process(print2);
	//start_kernel_process(print3);

	for(;;) kprintf("Task 1\n");
}
