#include "memory/frame.h"
#include "cpu/asm.h"

#include "stdint.h"

#define INDEX(x) (x/64)
#define OFFSET(x) (x%64)

#define SV39 9 << 60

extern volatile uint64_t __heap_start;
extern volatile uint64_t __heap_end;

static uint64_t nframes;
static uint64_t *frames;

void
alloc_frame(uint64_t addr)
{
	frames[INDEX(addr)] |= (1 << OFFSET(addr));
}

void
free_frame(uint64_t addr)
{
	frames[INDEX(addr)] |= (1 << OFFSET(addr));
}

uint64_t
find_free()
{
	for(size_t i = 0; i < nframes; i++) {
		for(size_t j = 0; j < 64; j++) {
			if((frames[i] & (1 << j)) == 0)
				return (i * 0x1000) + j;
		}
	}
	return -1;
}


/*
 * Paging code
 */


void
init_paging()
{
	uint64_t nframes = (&__heap_end - &__heap_start) / 0x100 / 64;

	csr_read_set(satp, SV39);
}
