#include "memory/palloc.h"
#include "memory/paging.h"

#include "os.h"

#define NFRAMES (MEM_AVAIL) / 0x1000 / 64

#define INDEX(x) (x/64)
#define OFFSET(x) (x%64)

/**
 * The bitmap is only there to keep track of actually available RAM!
 */
static uint64_t frames[NFRAMES];

void
alloc_frame(uint64_t addr)
{
	if(addr < PAGE_FRAME_START)
		return;

	addr -= PAGE_FRAME_START;
	addr >>= 12;
	frames[INDEX(addr)] |= (1 << OFFSET(addr));
}

void
free_frame(uint64_t addr)
{
	if(addr < PAGE_FRAME_START)
		return;

	addr -= PAGE_FRAME_START;
	addr >>= 12;
	frames[INDEX(addr)] |= (1 << OFFSET(addr));
}

uint64_t
find_free()
{
	for(size_t i = 0; i < NFRAMES; i++) {
		for(size_t j = 0; j < 64; j++) {
			if((frames[i] & (1 << j)) == 0) {
				return PAGE_FRAME_START + ((i*64 + j) * 0x1000);
			}
		}
	}
	return -1;
}
