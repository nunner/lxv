#include "driver/virtio.h"
#include "driver/virtio-classes.h"

#include "os.h"
#include "memory/heap.h"
#include "memory/paging.h"

static char *types[] = {
	"None", // "Reserved"
	"Network card",
	"Block device",
	"Console",
	"Entropy source",
	"memory ballooning (traditional)",
	"ioMemory",
	"rpmsg",
	"SCSI host",
	"9P transport",
	"mac80211 wlan",
	"rproc serial",
	"virtio CAIF",
	"memory balloon",
	"GPU device",
	"Timer/lLock device"
	"Input device",
};

dev_setup_t	setup_functions[] = {
	0,
	setup_network,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};

static virtio_dev_t *devices[MMIO_COUNT];

void
setup_device(virtio_dev_t *dev)
{
	int id = read_virtio_field(uint32_t, dev, DeviceID);

	write_virtio_field(0, uint32_t, dev, Status);
		
	if(setup_functions[id] != 0)
		setup_functions[id](dev);
	else
		logf("No driver found for %s", types[id]);
}

void
scan_virtio()
{
	kprintf("Scanning virtio devices.\n");

	for(size_t i = MMIO_COUNT - 1; i > 0 ; i--) {
		virtio_dev_t *dev = (virtio_dev_t *) (MMIO_START + i * MMIO_STEP);

		if(read_virtio_field(uint32_t, dev, MagicValue) == MAGIC_VALUE) {
			logf("Bus %d: \t%s\n", MMIO_COUNT - i, types[read_virtio_field(uint32_t, dev, DeviceID)]);

			if(read_virtio_field(uint32_t, dev, DeviceID) != 0)
				devices[i] = dev;
		}
	}

	kprintf("Setting up devices.\n");
	for(size_t i = 0; i < MMIO_COUNT; i++) {
		if(devices[i] != 0)
			setup_device(devices[i]);
	}
}

uint64_t
sel_queue(virtio_dev_t *dev, uint64_t idx)
{
	write_virtio_field(idx, uint32_t, dev, QueueSel);

	if(!read_virtio_field(uint32_t, dev, QueueNumMax))
		panic("Queue is not ready.");

	return read_virtio_field(uint32_t, dev, QueueNumMax);
}


VirtQueue *
get_queue(virtio_dev_t *dev, uint64_t idx)
{
	sel_queue(dev, idx);
	return (VirtQueue *) read_virtio_field(uint64_t, dev, QueuePFN);
}

void
add_buffer(virtio_dev_t *dev, VirtQueue *queue)
{
	for(size_t i = 0; i < _QUEUE_NUM; i++) {
		if(queue->Buffers[i].Length == 0) {
			uint64_t buffer = (uint64_t) malloc(1500);	
			queue->Buffers[i].Address = virt_to_phys(buffer);
			queue->Buffers[i].Address = buffer;

			int index = queue->Available.Index % _QUEUE_NUM;

			queue->Available.Ring[index] = i;
			queue->Available.Index++;	

			write_virtio_field(index, uint32_t, dev, QueueNotify);
			break;
		}
	}
}

