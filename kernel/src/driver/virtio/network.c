#include "driver/virtio.h"
#include "driver/virtio/network.h"

#include "net/util.h"
#include "memory/palloc.h"

#include "os.h"

#include "stdbool.h"

static uint64_t packet;

// https://docs.oasis-open.org/virtio/virtio/v1.0/cs04/virtio-v1.0-cs04.pdf

void
setup_features(virtio_dev_t *dev)
{
	set_virtio_field_bit(VIRTIO_NET_F_MAC, uint32_t, dev, GuestFeatures);

	set_virtio_field_bit(FEATURES_OK, uint32_t, dev, Status);

	if(!(read_virtio_field(uint32_t, dev, Status) & FEATURES_OK))
		panic("Couldn't set up device.");
}

// Returns whether the queue is in use or not.
bool
sel_queue(virtio_dev_t *dev, int num)
{
	write_virtio_field(num, uint32_t, dev, QueueSel);

	if(!read_virtio_field(uint32_t, dev, QueueNumMax))
		panic("Queue is not ready.");

	return read_virtio_field(uint32_t, dev, QueuePFN);	
}

void
setup_queues(virtio_dev_t *dev)
{
	int i = 0; 
	while(sel_queue(dev, i) != 0) {
		uint64_t addr = find_free();	
		alloc_frame(addr);


		
	}
}

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
setup_network(virtio_dev_t *dev)
{
	log("Setting up a network card.\n");

	write_virtio_field(0, 				uint32_t, dev, Status);
	set_virtio_field_bit(ACKNOWLEDGE, 	uint32_t, dev, Status);
	set_virtio_field_bit(DRIVER, 		uint32_t, dev, Status);
	int id = DEV_ID(dev);	

	uint32_t features = read_virtio_field(uint32_t, dev, HostFeatures);
	logf("Features: %d\n", features);

	setup_features(dev);
	setup_queues(dev);

	set_virtio_field_bit(DRIVER_OK, uint32_t, dev, Status);

	virtio_net_config conf = read_virtio_field(virtio_net_config, dev, Config);
	kprintf("MAC: ");
	print_mac(conf.mac);

	notify(id, &packet);
	
	start_kernel_process(network);
}
