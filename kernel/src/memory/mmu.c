#include "memory/mmu.h"
#include "memory/heap.h"

extern volatile void *__heap_start;

heap_t *kernel_heap;

void
setup_heap()
{	
	kernel_heap = (heap_t *) __heap_start;	

	switch_heap(kernel_heap);
}
