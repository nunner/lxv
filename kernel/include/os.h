#pragma once

// General
#define MEM_AVAIL 0x8000000 

// Heap size at startup
#define HEAP_START_SIZE 0x10000

// Paging variables
#define PAGE_FRAME_START 0x80000000

#define MMIO_START 		0x10001000	
#define MMIO_STEP		0x1000
#define MMIO_COUNT 		7

#define LOGGING

#ifdef LOGGING
	#define log(s,...) kprintf(s, __VA_ARGS__)
#else
	#define log(s,...) 
#endif

#include "scheduler/mutex.h"

void
kprintf(char *s, ...);
