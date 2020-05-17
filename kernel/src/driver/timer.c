#include "driver/timer.h"

uint64_t
get_time()
{
	return *((uint64_t *) MTIME);	
}

void
set_limit(uint64_t val)
{
	*((uint64_t *) TIMECMP) = val;
}
