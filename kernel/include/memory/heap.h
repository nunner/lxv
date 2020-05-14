#pragma once

#include "stdint.h"
#include "stdbool.h"

typedef struct node {
	uint64_t size;
	bool active;
	struct node *next;
} node_t;


typedef struct {
	struct node *start;
	struct node *end;
	uint64_t size;
} heap_t;

void
switch_heap(heap_t *heap);

void *
malloc(size_t size);

void
free(void *);
