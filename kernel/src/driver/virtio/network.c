#include "driver/virtio.h"
#include "driver/virtio/network.h"

#include "net/util.h"

#include "os.h"

#include "stdbool.h"


// https://docs.oasis-open.org/virtio/virtio/v1.0/cs04/virtio-v1.0-cs04.pdf

void
setup_features(virtio_dev_t *dev)
{
	set_virtio_field_bit(VIRTIO_NET_F_MAC, uint32_t, dev, GuestFeaturesSel);

	set_virtio_field_bit(DRIVER_OK, uint32_t, dev, Status);	

	bool state = read_virtio_field(uint32_t, dev, Status) & DRIVER_OK;

	if(!state)
		panic("Couldn't negotiate features with network card.");
}

void
set_queues(virtio_dev_t *dev)
{
	(void) dev;	
}

void
setup_network(virtio_dev_t *dev)
{
	log("Setting up a network card.\n");

	write_virtio_field(0, 				uint32_t, dev, Status);
	set_virtio_field_bit(ACKNOWLEDGE, 	uint32_t, dev, Status);
	set_virtio_field_bit(DRIVER, 		uint32_t, dev, Status);

	uint32_t features = read_virtio_field(uint32_t, dev, GuestFeatures);
	logf("%d\n", features);

	setup_features(dev);
	set_queues(dev);

	set_virtio_field_bit(DRIVER_OK, uint32_t, dev, Status);

	virtio_net_config conf = read_virtio_field(virtio_net_config, dev, Config);
	kprintf("MAC: ");
	print_mac(conf.mac);
}
