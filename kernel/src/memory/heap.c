#include "memory/heap.h"

#include "memory/paging.h"
#include "memory/palloc.h"

static heap_t *current_heap;

void
switch_heap(heap_t *heap)
{
	if(heap != 0)
		current_heap = heap;
}

void *
kmalloc(size_t size)
{
	(void) size;

	return 0;
}

// TODO: make it account for different sizes.
void *
kmalloc_a(size_t size)
{
	(void) size;
	uint64_t ret = find_free();
	alloc_frame(ret);

	return (void *) ret;
}
