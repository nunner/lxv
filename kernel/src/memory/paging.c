#include "memory/paging.h"
#include "memory/kmalloc.h"
#include "cpu/asm.h"

#include "stdint.h"
#include "stdbool.h"
#include "string.h"

#include "driver/uart.h"

#define INDEX(x) (x/64)
#define OFFSET(x) (x%64)

#define VPN(va, lvl) (((va) >> POFFSET(lvl)) & 0x1FF)
#define POFFSET(lvl) (12 + 9*lvl)

#define PA(page) ((uint64_t) page->ppn << 12)

extern volatile void __heap_start;
extern volatile void __heap_end;
extern volatile void __uart;
extern volatile void __kernel_start;
extern volatile void __kernel_end;

uint64_t placement_addr;

static uint64_t nframes;
static uint64_t *frames;

static pt_t *root_table;

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

pte_t *
find(pt_t *pagetable, uint64_t va, bool create)
{
	pte_t *page;
	pt_t  *current = pagetable;

	for(int i = 2; i > 0; i--) {
		page = &current->pages[VPN(va, i)];

		if(page == 0 || !(page->flags & PTE_V)) {
			if(!create) return 0;

			pt_t *table = (pt_t *) kmalloc_a(sizeof(pt_t));
			memset(table, 0, sizeof(pt_t)); 
			page->ppn = (uint64_t) table >> 12;
			page->flags |= PTE_V;
		}

		current = (pt_t *) PA(page);
	}

	return &current->pages[VPN(va, 0)];
}

void
map_range(pt_t *pagetable, uint64_t vaddr, uint64_t paddr, uint64_t size, uint64_t flags)
{
	for(size_t i = 0; i <=  size; i += PAGE_SIZE)
	{
		uint64_t pos = vaddr + i;
		pte_t *page = find(pagetable, pos, TRUE);
		page->flags = flags;
		page->ppn = (uint64_t) ((paddr + i) >> 12);
		page->flags |= PTE_V;
	}
}


void
init_paging()
{
	placement_addr = (uint64_t) &__heap_start;

	uint64_t nframes = (&__heap_end - &__heap_start) / 0x100 / 64;
	frames = (uint64_t *) kmalloc_a(sizeof(uint64_t) * nframes);

	root_table = (pt_t *) kmalloc_a(sizeof(pt_t));
	memset(root_table, 0, sizeof(pt_t));

	map_range(root_table, (uint64_t) &__kernel_start, (uint64_t) &__kernel_start,  (uint64_t ) &__kernel_end - (uint64_t) &__kernel_start, PTE_W | PTE_R);
	 map_range(root_table, (uint64_t) &__uart, (uint64_t) &__uart, 0x100, PTE_W | PTE_R);
	map_range(root_table, (uint64_t) &__heap_start, (uint64_t) &__heap_start, (uint64_t) &__heap_end - (uint64_t) &__heap_start, PTE_W | PTE_R);

	csr_set(satp, (uint64_t) root_table >> 12);
}
