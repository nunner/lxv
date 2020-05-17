#pragma once

// General
#define MEM_AVAIL 0x8000000 

// Heap size at startup
#define HEAP_START_SIZE 0xF000

// Paging variables
#define PAGE_FRAME_START 0x80000000

#define LOGGING

void
kprintf(char *s, ...);

