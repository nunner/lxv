#pragma once

#define MTIME 	0x0200bff8
#define TIMECMP 0x02004000 

#define FREQUENCY 1000000

#include "stdint.h"

uint64_t
get_time();

void
set_limit(uint64_t val);
