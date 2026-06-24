#include "transfer.h"

#include "mpu6050.h"

#include <errno.h>
#include <string.h>

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>

int mpu6050_transfer(void *ctx, uint8_t reg, uint8_t *buf, size_t len, bool read)
{
	const struct device *dev = ctx;
	const struct mpu6050_config *config = dev->config;

	if (read) {
		return i2c_write_read_dt(&config->bus, &reg, 1, buf, len);
	}

	uint8_t tx[1 + 32];

	if (len > sizeof(tx) - 1) {
		return -EINVAL;
	}

	tx[0] = reg;
	memcpy(&tx[1], buf, len);

	return i2c_write_dt(&config->bus, tx, len + 1);
}

int mpu6050_transfer_init(const struct device *dev)
{
	const struct mpu6050_config *config = dev->config;

	if (!i2c_is_ready_dt(&config->bus)) {
		return -ENODEV;
	}

	return 0;
}
