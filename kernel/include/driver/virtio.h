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

typedef enum {
	ACKNOWLEDGE			= 1,
	DRIVER				= 2,
	DRIVER_OK			= 4,
	FEATURES_OK			= 8,
	DEVICE_NEEDS_RESET 	= 64,
	FAILED				= 128,
} driver_status;

typedef void (*dev_setup_t)(virtio_dev_t *);

#define read_virtio_field(type, dev, field) *((type *) ((uint64_t) dev + field))
#define write_virtio_field(value, type, dev, field) *((type *) ((uint64_t) dev + field)) = value
#define set_virtio_field_bit(bit, type, dev, field) write_virtio_field(read_virtio_field(type, dev, field) | bit, type, dev, field);
#define FEATURE(name, id) name = id,

#define _DEV_MASK 0xF000
#define _DEV_ID(dev) (dev & _DEV_MASK) / 0x1000
#define DEV_ID(dev) (_DEV_ID((uint64_t) dev))

void
scan_virtio();
