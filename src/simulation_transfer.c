#include "transfer.h"

#include "__DRIVER_SLUG__.h"

#include <errno.h>

#include <zephyr/device.h>

int __DRIVER_SLUG___transfer(void *ctx, uint8_t reg, uint8_t *buf, size_t len, bool read)
{
	ARG_UNUSED(ctx);
	ARG_UNUSED(reg);
	ARG_UNUSED(buf);
	ARG_UNUSED(len);
	ARG_UNUSED(read);

	/* FILL IN: serialize register read/write, send over socket, block for response */

	return -ENOTSUP;
}

int __DRIVER_SLUG___transfer_init(const struct device *dev)
{
	ARG_UNUSED(dev);

	/* FILL IN: open socket to Basilisk and store fd in dev->data if needed */

	return 0;
}
