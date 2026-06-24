#include "transfer.h"

#include "__DRIVER_SLUG__.h"

#include <errno.h>
#include <string.h>

#include <zephyr/device.h>

#define __DRIVER_UPPER___REGISTER_MAP_SIZE 256

static uint8_t register_map[__DRIVER_UPPER___REGISTER_MAP_SIZE];


int __DRIVER_SLUG___transfer(void *ctx, uint8_t reg, uint8_t *buf, size_t len, bool read)
{
	ARG_UNUSED(ctx);

	if (reg + len > sizeof(register_map)) {
		return -EINVAL;
	}

	if (read) {
		memcpy(buf, &register_map[reg], len);
	} else {
		memcpy(&register_map[reg], buf, len);
	}

	return 0;
}


int __DRIVER_SLUG___transfer_init(const struct device *dev)
{
	ARG_UNUSED(dev);

	memset(register_map, 0, sizeof(register_map));

	return 0;
}
