#pragma once

#include "stdint.h"

void
handle_supervisor_trap() __attribute__((interrupt));

void
enable_supervisor_interrupts();
