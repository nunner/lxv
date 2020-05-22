#include "driver/virtio.h"
#include "driver/virtio/network.h"

#include "os.h"

static int id;

// https://docs.oasis-open.org/virtio/virtio/v1.0/cs04/virtio-v1.0-cs04.pdf

void
setup_network(virtio_dev_t *dev)
{
	id = get_virtio_field(uint32_t, dev, DeviceID);
	log("Setting up a network card.\n","");

	
}
