#include "net/util.h"

#include "os.h"

#define CHAR_HEX(x) ((x) > 9 ? ('A' - 10 + (x)) : ('0' + (x)))

void
print_mac(const uint8_t *mac)
{
	if(mac == 0) return;

	for(size_t i = 0; i < 5; i++)
		kprintf("%c%c:", CHAR_HEX(mac[i] >> 4), CHAR_HEX(mac[i] & 0xF));

	kprintf("%c%c", CHAR_HEX(mac[5] >> 4), CHAR_HEX(mac[5] & 0xF));
}
