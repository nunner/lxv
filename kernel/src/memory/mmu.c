#include "memory/mmu.h"

#include "os.h"
#include "memory/heap.h"
#include "memory/palloc.h"


extern volatile void __heap_start;

heap_t *kernel_heap;

heap_t *
setup_heap()
{	
	kernel_heap = (heap_t *) &__heap_start;	

	kernel_heap->size = HEAP_START_SIZE;
	kernel_heap->start = (node_t *) (&__heap_start + sizeof(heap_t));
	kernel_heap->end = (node_t *) (&__heap_start + kernel_heap->size - sizeof(heap_t));

	kernel_heap->start->next = kernel_heap->end;
	kernel_heap->start->size = ((uint64_t) kernel_heap->end - (uint64_t) kernel_heap->start);
	kernel_heap->start->active = FALSE;

	switch_heap(kernel_heap);
	return kernel_heap;
}

void *
kmalloc_a(size_t size)
{
	(void) size;
	uint64_t ret = find_free();
	alloc_frame(ret);

	return (void *) ret;
}
