#include "driver/uart.h"
#include "stdint.h"

// TODO: fix __uart symbol stuff
extern volatile void __uart;
uint8_t *control;

// Refer to 16550D docs
void
setup_uart()
{
	control = (uint8_t *) &__uart;

	// Set transmission length to 8 bits
	control[3] = 0b11;
	
	// Enable FIFO mode
	control[2] = 0b1;

	control[1] = 0b1;
}

void 
uart_cwrite(char c)
{
	control[0] = c;
}

void
uart_write(char *s)
{
	do {
		uart_cwrite(*s);
	} while(*s++ != '\0');
}

char
uart_cread()
{
	if((control[5] & 0b1) != 0)
		return control[0];
	else
		return 0;
}
