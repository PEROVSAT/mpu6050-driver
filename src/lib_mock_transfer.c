#include "transfer.h"

#include "mpu6050.h"

#include <errno.h>
#include <string.h>

#include <zephyr/device.h>

#define MPU6050_REGISTER_MAP_SIZE 256

static uint8_t register_map[MPU6050_REGISTER_MAP_SIZE];

int mpu6050_transfer(void *ctx, uint8_t reg, uint8_t *buf, size_t len, bool read)
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

int mpu6050_transfer_init(const struct device *dev)
{
	ARG_UNUSED(dev);

	memset(register_map, 0, sizeof(register_map));
	register_map[0x75] = 0x68; /* WHO_AM_I: MPU6050 chip ID */
	register_map[0x6B] = 0x40; /* PWR_MGMT1: sleep bit set (cleared by lib init) */
	register_map[0x3F] = 0x40; /* ACCEL_ZOUT_H: high byte of 1g at ±2g FS */
	register_map[0x40] = 0x00; /* ACCEL_ZOUT_L */

	return 0;
}
