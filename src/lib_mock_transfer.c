#include "mpu6050_bus.h"
#include "mpu6050_priv.h"

#include <errno.h>

int mpu6050_transfer(void *ctx, uint8_t reg, uint8_t *buf, size_t len, bool read)
{
	ARG_UNUSED(ctx);
	ARG_UNUSED(reg);
	ARG_UNUSED(buf);
	ARG_UNUSED(len);
	ARG_UNUSED(read);

	/* FILL IN: in-memory stand-in for the bus, so lib/ can run without hardware. */

	return -ENOTSUP;
}

int mpu6050_transfer_init(const struct device *dev)
{
	ARG_UNUSED(dev);

	return 0;
}
