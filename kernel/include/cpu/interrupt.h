#pragma once

#include "stdint.h"

enum {
	MACHINE_EXTERNAL = 11,
} mcause;

void
default_handler(uint64_t code);

extern void
enable_interrupts();

extern void
enable_machine_interrupt(int code);
