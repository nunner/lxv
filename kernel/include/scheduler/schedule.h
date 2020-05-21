#pragma once

#include "stdint.h"

typedef struct {
	uint64_t x[32];
	uint64_t satp;
	uint64_t sp;
	uint64_t pc;
} frame_t;

typedef enum {
	RUNNING,
	SLEEPING,
	STOPPED
} state_t;

typedef struct process {
	uint64_t pid;
	frame_t frame;
	state_t state;
	uint64_t stack[4096];
	struct process *next;
} process_t;

void
schedule();
