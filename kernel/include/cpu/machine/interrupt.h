#pragma once

#include "stdint.h"

void
handle_machine_trap() __attribute__((interrupt));

void
enable_machine_interrupts();
