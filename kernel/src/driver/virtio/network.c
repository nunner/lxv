#include "driver/virtio.h"
#include "driver/virtio/network.h"

#include "net/util.h"
#include "memory/palloc.h"
#include "memory/paging.h"
#include "memory/heap.h"

#include "os.h"

#include "stdbool.h"

extern void start_networking(virtio_dev_t *dev);

// https://docs.oasis-open.org/virtio/virtio/v1.0/cs04/virtio-v1.0-cs04.pdf

void
setup_features(virtio_dev_t *dev)
{
	set_virtio_field_bit(VIRTIO_NET_F_MAC, uint32_t, dev, GuestFeatures);
	set_virtio_field_bit(VIRTIO_NET_F_MTU, uint32_t, dev, GuestFeatures);

	set_virtio_field_bit(FEATURES_OK, uint32_t, dev, Status);

	if(!(read_virtio_field(uint32_t, dev, Status) & FEATURES_OK))
		panic("Couldn't set up device: queue isn't ready");
}

// Returns whether the queue is in use or not.
void
setup_queues(virtio_dev_t *dev)
{
	write_virtio_field(PAGE_SIZE, uint32_t, dev, GuestPageSize);

	for(size_t i = 0; i < 2; i++) {
		int size = sel_queue(dev, i);
		if(_QUEUE_NUM > size)  
			panic("Couldn't set up device: size");
		write_virtio_field(_QUEUE_NUM, uint32_t, dev, QueueNum);

		VirtQueue *queue = (VirtQueue *) get_page();
		// We don't need to translate the address because the page allocator
		// simply identity-maps it.
		write_virtio_field((uint64_t) queue, uint64_t, dev, QueuePFN);
		write_virtio_field(PAGE_SIZE, uint32_t, dev, QueueAlign);
		add_buffer(dev, queue);
	}
}

// This is very, very janky rn. Please don't try to use multiple NICs.
void
setup_network(virtio_dev_t *dev)
{
	log("Setting up a network card.\n");

	write_virtio_field(0, 				uint32_t, dev, Status);
	set_virtio_field_bit(ACKNOWLEDGE, 	uint32_t, dev, Status);
	set_virtio_field_bit(DRIVER, 		uint32_t, dev, Status);

	uint32_t features = read_virtio_field(uint32_t, dev, HostFeatures);
	logf("Features: %d\n", features);

	setup_features(dev);
	setup_queues(dev);

	set_virtio_field_bit(DRIVER_OK, uint32_t, dev, Status);

	start_networking(dev);
}
