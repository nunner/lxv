#pragma once

#include "stdint.h"

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
	uint16_t ppn0 : 9;		//
	uint16_t ppn1 : 9;		//
	uint32_t ppn2 : 26;		//
	uint16_t reserved : 10; //
} pte_t;

typedef struct __attribute__((packed))
{
	pte_t pages[512];
} pt_t;
