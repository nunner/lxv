#include "net/net.h" 

#include "net/util.h"
#include "driver/virtio/network.h"

#include "os.h"

#include "stdbool.h"

static uint64_t packet;
static virtio_dev_t *current_dev;

void
network()
{
	for(;;) {
		if(packet) {
			packet = FALSE;
			log("Packet!");
		}
	}
}

void
start_networking(virtio_dev_t *dev) 
{
	int id = DEV_ID(dev);

	virtio_net_config conf = read_virtio_field(virtio_net_config, dev, Config);
	kprintf("MAC: ");
	print_mac(conf.mac);

	current_dev = dev;

	notify(id, &packet);
	start_kernel_process(network);
}
