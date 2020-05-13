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

void
resize(heap_t *heap);

void *
malloc(size_t size)
{
	(void) size;

	node_t *curr = current_heap->start;
	node_t *temp;	

	while(curr->next != 0) {
		if(!curr->active && curr->size >= size) {
			temp = curr->next;
			curr->active = TRUE;

			curr->next = (node_t *) (curr + sizeof(curr) + size);	
			curr->next->next = temp;
			curr->next->active = FALSE;

			return curr;
		}

		curr = curr->next;
	}

	return 0;
}
