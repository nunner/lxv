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
clean(heap_t *heap)
{
	(void) heap;
	
	node_t *curr = current_heap->start;
	node_t *temp;	

	while(curr->next != 0) {
		if(!curr->active) {
			temp = curr;
			while(temp->next != 0 && !temp->active)
				temp = temp->next;
			curr->next = temp;
		}
		
		curr = curr->next;
	}
}

void
expand(heap_t *heap, size_t size)
{
	size &= ~(0x1FFF);
	size += PAGE_SIZE;

	uint64_t placement = (uint64_t) heap + heap->size;

	for(size_t i = 0; i < size; i += PAGE_SIZE) {
		request_page(placement);
		placement += PAGE_SIZE;
	}

	node_t *last_node = (node_t *) placement - sizeof(node_t);
	current_heap->end->next = last_node;
	current_heap->end = last_node;
	
	clean(current_heap);
}

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

			curr->next = (node_t *) (curr + sizeof(node_t) + size);	
			curr->next->next = temp;
			curr->next->active = FALSE;

			return curr + sizeof(node_t);
		}

		curr = curr->next;
	}

	return malloc(size);
}

void
free(void *ptr)
{
	node_t *node = (node_t *) ptr - sizeof(node_t);
	node->active = 0;
	clean(current_heap);
}

