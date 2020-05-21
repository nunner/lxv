#pragma once

#include "stdint.h"

#define MAGIC_VALUE 0x74726976

typedef void virtio_dev_t;

typedef enum {
	MagicValue 		= 0x000,
	Version 		= 0x004,
	DeviceID 		= 0x008,
	VendorID		= 0x00c,
	HostFeatures	= 0x010,
	HostFeaturesSel	= 0x014,
	GuestFeatures	= 0x020,
	GuestFeaturesSel= 0x024,
	GuestPageSize	= 0x028,
	QueueSel		= 0x030,
	QueueNumMax		= 0x034,
	QueueNum 		= 0x038,
	QueueAlign		= 0x03c,
	QueuePFN 		= 0x040,
	QueueNotify 	= 0x050,
	InterruptStatus = 0x060,
	InterruptAck 	= 0x064,
	Status 			= 0x070,
	Config 			= 0x100,

} offset;

typedef void (*dev_setup_t)(virtio_dev_t *);

#define get_virtio_field(type, dev, field) *((type* ) ((uint64_t) dev + field))
#define write_virtio_field(value, type, dev, field) *((type* ) ((uint64_t) dev + field)) = value

void
scan_virtio();
