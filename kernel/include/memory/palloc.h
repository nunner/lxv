#pragma once

#include "stdint.h"

void
alloc_frame(uint64_t addr);

uint64_t
find_free();

void *
get_page();
