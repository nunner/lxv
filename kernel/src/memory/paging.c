#include "memory/paging.h"

#include "os.h"

#include "cpu/asm.h"

#include "driver/uart.h"
#include "driver/timer.h"

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
extern volatile void __heap_start;
extern volatile void __placement_addr;
extern volatile void __plic;

static pte_t *placement;
static bool enabled = FALSE;

pt_t *root_table;
pt_t *current_table;

/*
 * Paging code
 */


/*
 * Flush the page cache so that new mappings are readable.
 */

#define flush(x) __asm__("sfence.vma x0, x0")

/**
 * What this does is a little weird. I have one page where I map 
 * all the things to I need while allocating new pages. This changes
 * the pointer of the original thingy and restores it when it's done.
 */
void
map_object(uint64_t addr)
{
	if(!enabled) return;

	placement->ppn = (addr >> 12);
	placement->flags = PTE_W | PTE_R | PTE_V;
	flush((uint64_t) &__placement_addr);
}

pte_t *
find(pt_t *pagetable, uint64_t va, bool create)
{
	if(enabled) lock();
	pte_t *page;
	pt_t  *current = pagetable;

	if(enabled) {
		map_object((uint64_t) pagetable);
		current = (pt_t *) &__placement_addr;
	}

	// May god forgive me, for I have sinned.

	for(size_t i = 2; i > 0; i--) {
		page = &current->pages[VPN(va, i)];

		if(page == 0 || !(page->flags & PTE_V)) {
			if(!create) {
				if(enabled) unlock();
				return 0;
			}
			
			uint64_t addr = (uint64_t) kmalloc_a(sizeof(pt_t));

			page->ppn = addr >> 12;
			page->flags |= PTE_V;
		}
		map_object(PA(page));

		if(!enabled) current = (pt_t *) PA(page);
	}
	
	if(enabled) unlock();
	return &current->pages[VPN(va, 0)];
}

void
map_range_at(uint64_t vaddr, uint64_t paddr, uint64_t size, uint64_t flags)
{
	pte_t *page;

	vaddr &= ~(0x1FFF);
	for(size_t i = 0; i <= size; i += PAGE_SIZE)
	{
		// Mark the position as used if the phsyical addr is located in physical RAM
		alloc_frame(paddr);

		uint64_t pos = vaddr + i;
		page = find(current_table, pos, TRUE);

		page->flags = flags;
		page->ppn = (uint64_t) ((paddr + i) >> 12);
		page->flags |= PTE_V;
		if(enabled) flush(pos);
	}
}

void
map_range_at_table(pt_t *pagetable, uint64_t vaddr, uint64_t paddr, uint64_t size, uint64_t flags)
{
	vaddr &= ~(0x1FFF);
	for(size_t i = 0; i < size; i += PAGE_SIZE)
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
map_range(uint64_t vaddr, uint64_t size, uint64_t flags)
{
	vaddr &= ~(0x1FFF);
	for(size_t i = 0; i < size; i += PAGE_SIZE)
	{
		uint64_t pos = vaddr + i;
		uint64_t ptr = find_free();
		map_range_at(pos, ptr, PAGE_SIZE, flags);
	}
}


// Translate a virtual address to a physical one
uint64_t
virt_to_phys(uint64_t addr)
{
	pte_t *page = find(current_table, addr, FALSE);
	if(page != 0)
		return (PA(page)) + (addr & 0xFFF);
	else
		return 0;
}

uint64_t
m_virt_to_phys(uint64_t addr)
{
	pte_t *page;
	pt_t *current = current_table;

	for(int64_t i = 2; i >= 0; i--) {
		page = &current->pages[VPN(addr, i)];
		current = (pt_t *) PA(page);
	}

	if(page != 0)
		return (PA(page)) + (addr & 0xFFF);
	else
		return 0;
}

void
switch_table(pt_t *table)
{
	if(table == 0)
		return;

	current_table = table;
	csr_set(satp, (uint64_t) table >> 12);
	csr_read_set(satp, (uint64_t) 8 << 60);
}

void
init_paging()
{ 
	for(size_t i = (uint64_t) &__kernel_start; i < (uint64_t) &__kernel_end; i += PAGE_SIZE)
		alloc_frame(i);

	alloc_frame((uint64_t) &__placement_addr);

	root_table = (pt_t *) kmalloc_a(sizeof(pt_t));
	memset(root_table, 0, sizeof(pt_t));

	// Don't use switch_table yet because we don't want to invalidate the caches… I think
	current_table = root_table;

	placement = find(root_table, (uint64_t) &__placement_addr, TRUE);	

	map_range((uint64_t) &__heap_start, HEAP_START_SIZE, PTE_W | PTE_R);
	map_range_at((uint64_t) placement, (uint64_t) placement, PAGE_SIZE, PTE_W | PTE_R);
	map_range_at((uint64_t) &__kernel_start, (uint64_t) &__kernel_start,  (uint64_t ) &__kernel_end - (uint64_t) &__kernel_start, PTE_W | PTE_R | PTE_X);
	map_range_at(MMIO_START, MMIO_START, MMIO_COUNT * MMIO_STEP, PTE_W | PTE_R);
	map_range_at((uint64_t) &__uart, (uint64_t) &__uart, 0x100, PTE_W | PTE_R);

	switch_table(root_table);
	enabled = TRUE;	
}
