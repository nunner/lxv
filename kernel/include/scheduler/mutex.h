#pragma once

#include "stdint.h"

extern void aquire(uint64_t);
extern void release(uint64_t);

#define CREATE_MUTEX(x) volatile uint64_t x;
#define MUTEX_LOCK(x) aquire((uint64_t) &x);
#define MUTEX_UNLOCK(x) release((uint64_t) &x);
