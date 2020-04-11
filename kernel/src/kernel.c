#include "os.h"

#include "cpu/cpu.h"
#include "driver/uart.h"

#include "memory/frame.h"

void 
main()
{
	setup_uart();
	uart_write("Hello, world\n");

	while(1)
		__asm__("wfi");
}
