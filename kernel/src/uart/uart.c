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

	char val;	
	// Set transmission length to 8 bits
	val = 0b11;
	// Set line control register
	control[3] = val;
	
	// Enable FIFO mode
	val = 0b1;
	control[2] = val;

	val = 0b1111;
	control[1] = val;

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
		return -1;
}
