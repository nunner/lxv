#include "memory/kmalloc.h"
#include "memory/paging.h"

#include "stdint.h"
#include "stdbool.h"

extern volatile void __heap_end;

extern uint64_t placement_addr;

void *
kmalloc_int(size_t size, bool align)
{
	if(align) 
		placement_addr = (placement_addr & ~(0x1FFF)) + PAGE_SIZE;
	void *ptr = (void *) placement_addr;
	placement_addr += size;
	return ptr;
} 

void *
kmalloc(size_t size)
{
	return kmalloc_int(size, FALSE);
}

void *
kmalloc_a(size_t size)
{
	return kmalloc_int(size, TRUE);
}
