#include "os.h"

#include "cpu/asm.h"

#include "cpu/supervisor/interrupt.h"
#include "driver/uart.h"
#include "scheduler/proc.h"
#include "memory/mmu.h"
#include "memory/paging.h"
#include "memory/palloc.h"

void
do_nothing_1()
{
	for(;;)
	kprintf("Process 1\n");
}	

void
do_nothing_2()
{
	for(;;)
	kprintf("Process 2\n");
}

void 
main()
{
	// Setup the UART
	setup_uart();
	enable_supervisor_interrupts();

	// Enable Sv39 paging
	init_paging();
	setup_heap();

	kprintf("Kernel task.");

	start_kernel_process(do_nothing_1);
	start_kernel_process(do_nothing_2);

	for(;;);
}
