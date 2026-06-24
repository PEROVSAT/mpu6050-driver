#include "transfer.h"

#include "mpu6050.h"

#include <errno.h>

#include <zephyr/device.h>

int mpu6050_transfer(void *ctx, uint8_t reg, uint8_t *buf, size_t len, bool read)
{
	ARG_UNUSED(ctx);
	ARG_UNUSED(reg);
	ARG_UNUSED(buf);
	ARG_UNUSED(len);
	ARG_UNUSED(read);

	/* FILL IN: serialize register read/write, send over socket, block for response */

	return -ENOTSUP;
}

int mpu6050_transfer_init(const struct device *dev)
{
	ARG_UNUSED(dev);

	/* FILL IN: open socket to Basilisk and store fd in dev->data if needed */

	return 0;
}
