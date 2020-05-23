#pragma once

#define INTERRUPT 0x8000000000000000
#define CODE ~(INTERRUPT)

#define REGISTER_INTERRUPT(interrupt, handler) {\
	interrupts[interrupt] = handler; \
}

#define REGISTER_EXCEPTION(exception, handler) {\
	exceptions[exception] = handler; \
}
