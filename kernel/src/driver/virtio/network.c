#include "driver/virtio.h"
#include "driver/virtio/network.h"

#include "os.h"

static int id;

void
setup_network(virtio_dev_t *dev)
{
	id = get_virtio_field(uint32_t, dev, DeviceID);
	log("Setting up a network card.\n","");
}
