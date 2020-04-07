#include "os.h"

#include "cpu/cpu.h"
#include "driver/uart.h"

void 
main()
{
	setup_uart();
	setup_interrupts();	

	uart_write("Hello, world\n");
	while(1)
		__asm__("wfi");
}
