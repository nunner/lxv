#pragma once

#include "driver/virtio.h"

#include "stdint.h"

typedef enum {
	FEATURE(VIRTIO_NET_F_CSUM, 	0)
	FEATURE(VIRTIO_NET_F_MAC, 	5)
} network_feature_t;

typedef struct {
	uint8_t mac[6];
} virtio_net_config;

void
setup_network(virtio_dev_t *dev);
