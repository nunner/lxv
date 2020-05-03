#pragma once

#include "stdint.h"

/*
 * TODO: use struct as soon as memory allocation works
 */ 
typedef struct {
	uint32_t *priority;
	uint32_t *pending;
	uint32_t *enable;
	uint32_t *treshold;
} split;

void
register_plic_interrupt(int i);

void
init_plic();

void
plic_handler();

void
set_treshold(int i);
