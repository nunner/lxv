#include "cpu/supervisor/plic.h"
#include "cpu/cpu.h"
#include "cpu/asm.h"

#include "stdint.h"

#define INDEX(x) (x/32)
#define BIT(x) (x%32)

extern volatile void __plic;

static uint32_t *priority;
static uint32_t *pending;
static uint32_t *enable;
static uint32_t *treshold;

void
init_plic()
{
	priority	= (uint32_t *) &__plic;
	pending		= (uint32_t *) &__plic + 0x1000;
	enable 		= (uint32_t *) &__plic + 0x2000;
	treshold 	= (uint32_t *) &__plic + 0x0C200000;
}

void
plic_handler()
{

}

void
set_treshold(int i)
{
	treshold[0] = i;
}

void
register_plic_interrupt(int i)
{
	enable[INDEX(i)] |= (1 << BIT(i));
	priority[i] = 1;
}
