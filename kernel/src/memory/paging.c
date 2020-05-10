#include "memory/paging.h"

#include "os.h"

#include "cpu/asm.h"

#include "driver/uart.h"

#include "memory/heap.h"
#include "memory/mmu.h"
#include "memory/palloc.h"

#include "stdint.h"
#include "stdbool.h"
#include "string.h"

#define VPN(va, lvl) (((va) >> POFFSET(lvl)) & 0x1FF)
#define POFFSET(lvl) (12 + 9*lvl)

#define PA(page) ((uint64_t) page->ppn << 12)

extern volatile void __uart;
extern volatile void __kernel_start;
extern volatile void __kernel_end;

static pte_t *placement;
static uint64_t placement_addr;
static bool enabled;

static pt_t *root_table;
pt_t *current_table;

/*
 * Paging code
 */

extern void
fence(uint64_t addr);

void
switch_table(pt_t *t);

void
set_placeholder(uint64_t paddr)
{
	placement->ppn = (paddr >> 12);
	placement->flags = PTE_W | PTE_R | PTE_V;
	fence(paddr);
}

pte_t *
find(pt_t *pagetable, uint64_t va, bool create)
{
	pte_t *page;
	pt_t  *current = pagetable;

	for(int i = 2; i > 0; i--) {
		page = &current->pages[VPN(va, i)];
		if(enabled) set_placeholder((uint64_t) page);
		if(page == 0 || !(page->flags & PTE_V)) {
			if(!create) return 0;

			pt_t *table = (pt_t *) kmalloc_a(sizeof(pt_t));
			if(enabled) set_placeholder((uint64_t) table);
			memset(table, 0, sizeof(pt_t)); 
			if(enabled) set_placeholder((uint64_t) page);
			page->ppn = (uint64_t) table >> 12;
			page->flags |= PTE_V;
		}

		if(enabled) current = (pt_t *) placement_addr;
		else 		current = (pt_t *) PA(page);
	}

	return &current->pages[VPN(va, 0)];
}

void
map_range(uint64_t vaddr, uint64_t paddr, uint64_t size, uint64_t flags)
{
	vaddr &= ~(0x1FFF);
	for(size_t i = 0; i <= size; i += PAGE_SIZE)
	{
		// Mark the position as used if the phsyical addr is located in physical RAM
		alloc_frame(paddr);

		uint64_t pos = vaddr + i;
		pte_t *page = find(current_table, pos, TRUE);
		page->flags = flags;
		page->ppn = (uint64_t) ((paddr + i) >> 12);
		page->flags |= PTE_V;
	}

	__asm__("sfence.vma");
}

void
map_range_table(pt_t *pagetable, uint64_t vaddr, uint64_t paddr, uint64_t size, uint64_t flags)
{
	vaddr &= ~(0x1FFF);
	for(size_t i = 0; i <= size; i += PAGE_SIZE)
	{
		uint64_t pos = vaddr + i;
		alloc_frame(pos);
		pte_t *page = find(pagetable, pos, TRUE);
		page->flags = flags;
		page->ppn = (uint64_t) ((paddr + i) >> 12);
		page->flags |= PTE_V;
	}
}

void
switch_table(pt_t *table)
{
	if(table == 0)
		return;

	current_table = table;
	csr_set(satp, (uint64_t) table >> 12);
	__asm__("sfence.vma");
}

void
init_paging()
{
	for(size_t i = (uint64_t) &__kernel_start; i < (uint64_t) &__kernel_end; i += PAGE_SIZE)
		alloc_frame(i);

	root_table = (pt_t *) kmalloc_a(sizeof(pt_t));
	memset(root_table, 0, sizeof(pt_t));

	// Don't use switch_table yet because we don't want to invalidate the caches… I think
	current_table = root_table;

	placement_addr = (uint64_t) kmalloc_a(sizeof(uint64_t));
	placement = find(current_table, placement_addr, TRUE);

	map_range((uint64_t) &__kernel_start, (uint64_t) &__kernel_start,  (uint64_t ) &__kernel_end - (uint64_t) &__kernel_start, PTE_W | PTE_R);
	map_range((uint64_t) placement, (uint64_t) placement, PAGE_SIZE, PTE_W | PTE_R);
	//map_range((uint64_t) &__uart, (uint64_t) &__uart, 0x100, PTE_W | PTE_R);

	setup_heap();
	switch_table(root_table);
	csr_read_set(satp, (uint64_t) 8 << 60);
	enabled = TRUE;	

	map_range((uint64_t) &__uart, (uint64_t) &__uart, 0x100, PTE_W | PTE_R);
}
