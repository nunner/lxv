#include "cpu/cpu.h"
#include "cpu/plic.h"
#include "stdint.h"

#define INDEX(x) (x/32)
#define BIT(x) (x%32)

extern volatile void __plic;

static uint32_t *priority;
static uint32_t *pending;
static uint32_t *enable;

void
init_plic()
{
	priority 	= (uint32_t *) &__plic;
	pending 	= (uint32_t *) &__plic + 0x1000;
	enable 		= (uint32_t *) &__plic + 0x2000;
}

void
register_interrupt(int i)
{
	priority[i] = 1;
	enable[INDEX(i)] |= (1 << BIT(i));
}
