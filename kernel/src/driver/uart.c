#include "driver/uart.h"

#include "os.h"
#include "scheduler/schedule.h"
#include "cpu/syscall.h"

#include "stdint.h"
#include "stdlib.h"
#include "string.h"

#include <stdarg.h>

extern volatile void __uart;
uint8_t *control;

CREATE_MUTEX(CONSOLE_MUTEX);

// Refer to 16550D docs
void
setup_uart()
{
	control = (uint8_t *) &__uart;

	// Set transmission length to 8 bits
	control[3] = 0b11;
	

	// Enable FIFO mode
	control[2] = 0b1;

	// Interrupts
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
	while(*s != 0) {
		uart_cwrite(*s++);
	}
}

void
kprintf (char *s, ...)
{
	MUTEX_LOCK(CONSOLE_MUTEX);
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
                        uint64_t num = va_arg(args, uint64_t);
                        itoa(num, buff);
						uart_write(buff);
                    }
                        break;
                    case 's': {
                        char *s = va_arg(args, char *);
						uart_write(s);
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
	MUTEX_UNLOCK(CONSOLE_MUTEX);
}

char
uart_cread()
{
	if((control[5] & 0b1) != 0)
		return control[0];
	else
		return 0;
}
