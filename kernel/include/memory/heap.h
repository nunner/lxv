#pragma once

#include "stdint.h"
#include "stdbool.h"

struct {
	uint64_t size;
	bool active;
	struct heap_block *next;
} node;

typedef struct node node_t;

typedef struct {
	node_t *start;
	node_t *end;
	uint64_t size;
} heap_t;

void
switch_heap(heap_t *heap);

void *
kmalloc(size_t size);

void *
kmalloc_a(size_t size);
