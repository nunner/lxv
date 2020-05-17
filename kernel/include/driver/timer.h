#pragma once

#define MTIME 	0x0200bbff8
#define TIMECMP 0x02004000

#include "stdint.h"

uint64_t
get_time();

void
set_limit(uint64_t val);
