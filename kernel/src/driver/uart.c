#include "driver/uart.h"

#include "os.h"

#include "stdint.h"
#include "stdlib.h"

#include <stdarg.h>

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

void
kprintf (char *s, ...)
{
    char buff[256];

    va_list args;
    va_start(args, s);

    /*
     * This is some horrible code that no one should ever have to see
     */
    while(*s != '\0') {
        switch(*s) {
            case '%': {
                ++s;
                switch(*s) {
                    case 'd': {
                        uint32_t num = va_arg(args, uint32_t);
                        itoa(num, buff);
                        kprintf("%s", buff);
                    }
                        break;
                    case 's': {
                        char *s = va_arg(args, char *);
                        kprintf(s);
                    }
                        break;
                    case 'c': {
                        char c = va_arg(args, int); // because putting 'char' throws a warning
                        uart_cwrite(c);
                    }
                        break;
                    default:
                        --s;
                }
            }
				break;
            default:
                uart_cwrite(*s);
                break;
        }
        ++s;
    }
}

char
uart_cread()
{
	if((control[5] & 0b1) != 0)
		return control[0];
	else
		return 0;
}
