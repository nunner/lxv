#pragma once

#include "stdint.h"

#define SV39 9 << 60
#define PAGE_SIZE 0x1000

#define PTE_V 1 << 0
#define PTE_R 1 << 1
#define PTE_W 1 << 2
#define PTE_X 1 << 3
#define PTE_U 1 << 4
#define PTE_G 1 << 5
#define PTE_A 1 << 6
#define PTE_D 1 << 7

typedef struct __attribute__((packed))
{
	uint8_t v : 1;			// 
	uint8_t r : 1;			//
	uint8_t w : 1;			//
	uint8_t x : 1;			//
	uint8_t u : 1;			//
	uint8_t g : 1;			//
	uint8_t a : 1;			//
	uint8_t d : 1;			//
	uint8_t rsw : 2;		//
	uint64_t ppn : 44;		//
	uint16_t reserved : 10; //
} pte_t;

typedef struct __attribute__((packed))
{
	pte_t pages[512];
} pt_t;
