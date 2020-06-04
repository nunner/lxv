#include "cpu/machine/plic.h"

#include "os.h"

#include "cpu/cpu.h"
#include "cpu/asm.h"
#include "memory/paging.h"

#include "stdint.h"
#include "stdbool.h"

extern volatile void __plic;

static uint32_t *priority;
static uint32_t *pending;
static uint32_t *enable;
static uint32_t *treshold;
static uint32_t *claim;

static uint64_t *notifications[32];

void
notify(uint64_t plic, uint64_t *var)
{
	if(plic > 32)
		return;

	notifications[plic] = (void *) virt_to_phys((uint64_t) var);
}


void
init_plic()
{
	priority	= (uint32_t *) ((uint64_t) &__plic);
	pending		= (uint32_t *) ((uint64_t) &__plic + 0x1000);
	enable 		= (uint32_t *) ((uint64_t) &__plic + 0x2000);
	treshold 	= (uint32_t *) ((uint64_t) &__plic + 0x200000);
	claim 		= (uint32_t *) ((uint64_t) treshold + 4);

	set_treshold(0);
}

void
handle_plic()
{
	uint32_t id;

	while((id = *claim) != 0) {
		if(notifications[id] != 0) {
			*notifications[id] = TRUE;	
		}
		*claim = id;
	}
}

void
set_treshold(int i)
{
	treshold[0] = i;
}

void
register_plic_interrupt(int i)
{
	*enable = *enable | (1 << i);
	priority[i] = 1;
}
