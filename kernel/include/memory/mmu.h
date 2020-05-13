#pragma once

#include "heap.h"

heap_t *
setup_heap();

// Why is this here? I don't know.
void *
kmalloc_a(size_t size);
