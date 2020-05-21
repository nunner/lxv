#pragma once

#include "stdint.h"

#define SV39 9 << 60
#define PAGE_SIZE 0x1000

#define PTE_V 1 << 0 // Valid
#define PTE_R 1 << 1 // Readable
#define PTE_W 1 << 2 // Writable
#define PTE_X 1 << 3 // Executable
#define PTE_U 1 << 4 // User space, available in user space
#define PTE_G 1 << 5 // Global mapping, available in all spaces
#define PTE_A 1 << 6 // Accessed
#define PTE_D 1 << 7 // Dirty

typedef struct __attribute__((packed))
{
	uint16_t flags : 8;
	uint8_t rsw : 2;		
	uint64_t ppn : 44;		
	uint16_t reserved : 10; 
} pte_t;

typedef struct __attribute__((packed))
{
	pte_t pages[512];
} pt_t;

#define PAGE_SIZE 0x1000
#define PAGE_MASK 0x1FFF

void
init_paging();

void
map_range_at(uint64_t vaddr, uint64_t paddr, uint64_t size, uint64_t flags);

void
map_range_at_table(pt_t *pagetable, uint64_t vaddr, uint64_t paddr, uint64_t size, uint64_t flags);

void
map_page(uint64_t vaddr, uint64_t flags);

uint64_t
virt_to_phys(uint64_t addr);

uint64_t
m_virt_to_phys(uint64_t addr);
