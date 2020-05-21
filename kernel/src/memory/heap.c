#include "memory/heap.h"

#include "os.h"

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
set_size(node_t *node)
{
	if(node->next != 0)
		node->size = (uint64_t) node->next - ((uint64_t) node + sizeof(node_t));
}

void
clean(heap_t *heap)
{
	
	node_t *curr = heap->start;
	node_t *temp;	

	while(curr->next != 0) {
		if(!curr->active) {
			temp = curr;
			while(temp->next != temp && temp->next != 0 && !temp->active)
				temp = temp->next;
			curr->next = temp;
			set_size(curr);

			// For some reason, this happens sometimes.
			if(temp->next == temp)
				temp->next = 0;
		}
		
		curr = curr->next;
	}
}

void
expand(heap_t *heap, size_t size)
{
	size &= ~(0x1FFF);
	size += PAGE_SIZE;

	uint64_t addr = (uint64_t) heap + heap->size;

	addr &= ~(0x1FFF);
	addr += PAGE_SIZE;

	for(size_t i = 0; i < size/PAGE_SIZE; i++) {
		request_page(addr);
		addr += PAGE_SIZE;
	}

	node_t *last_node = (node_t *) (addr - sizeof(node_t));
	current_heap->end->next = last_node;
	current_heap->end = last_node;

	heap->size += size;
	
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

			curr->next = (node_t *) ((uint64_t) curr + sizeof(node_t) + size);	
			curr->next->next = temp;
			curr->next->active = FALSE;

			set_size(curr);
			set_size(curr->next);

			return (void *) ((uint64_t) curr + sizeof(node_t));
		}

		curr = curr->next;
	}

	expand(current_heap, size + 2 * sizeof(node_t));

	return malloc(size);
}

void
free(void *ptr)
{
	node_t *node = (node_t *) ptr - sizeof(node_t);
	node->active = 0;
	clean(current_heap);
}

