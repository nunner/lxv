#pragma once

#include "stdint.h"

void
register_plic_interrupt(int i);

void
init_plic();

void
set_treshold(int i);

void
handle_plic();
