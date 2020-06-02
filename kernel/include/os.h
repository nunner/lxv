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
	#define logf(s,...) kprintf(s, __VA_ARGS__)
	#define log(s) kprintf(s);	
#else
	#define logf(s,...) 
	#define log(s) 
#endif

#include "scheduler/mutex.h"
#include "scheduler/proc.h"
#include "cpu/syscall.h"

void
kprintf(char *s, ...);

void
notify(uint64_t plic, uint64_t *var);

void
panic(char *s);
