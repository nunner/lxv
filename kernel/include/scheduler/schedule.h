#pragma once

#include "stdint.h"

typedef struct __attribute__((packed)){
	uint64_t x[31];
	uint64_t sp;
	uint64_t pc;
	// uint64_t satp;
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
