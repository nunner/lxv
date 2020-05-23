#pragma once

#include "stdint.h"
#include "scheduler/schedule.h"

extern void aquire(uint64_t);
extern void release(uint64_t);

#define __MUTEX volatile uint64_t
#define CREATE_MUTEX(x) __MUTEX x;
#define MUTEX_LOCK(x) aquire((uint64_t) &x);
#define MUTEX_UNLOCK(x) release((uint64_t) &x);

extern __MUTEX LOCK_MUTEX;

#define lock()		MUTEX_LOCK(LOCK_MUTEX)
#define unlock()	MUTEX_UNLOCK(LOCK_MUTEX)
