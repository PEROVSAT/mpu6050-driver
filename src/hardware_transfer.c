#include "transfer.h"

#include "__DRIVER_SLUG__.h"

#include <errno.h>

#include <zephyr/device.h>

/* FILL IN: include bus driver header, e.g. <zephyr/drivers/i2c.h> */

int __DRIVER_SLUG___transfer(void *ctx, uint8_t reg, uint8_t *buf, size_t len, bool read)
{
	ARG_UNUSED(ctx);
	ARG_UNUSED(reg);
	ARG_UNUSED(buf);
	ARG_UNUSED(len);
	ARG_UNUSED(read);

	/* FILL IN: perform bus read/write using config from dev->config */

	return -ENOTSUP;
}

int __DRIVER_SLUG___transfer_init(const struct device *dev)
{
	ARG_UNUSED(dev);

	/* FILL IN: verify bus is ready, e.g. i2c_is_ready_dt(&config->bus) */

	return 0;
}
