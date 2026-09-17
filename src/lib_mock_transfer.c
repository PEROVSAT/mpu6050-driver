#include "mpu6050_bus.h"
#include "mpu6050_priv.h"
#include "mpu6050_regs.h"

#include <errno.h>
#include <string.h>

#define MPU6050_REGISTER_MAP_SIZE 256

static uint8_t register_map[MPU6050_REGISTER_MAP_SIZE];

static void mpu6050_library_mock_init_once(void)
{
	static bool initialized;

	if (initialized) {
		return;
	}

	memset(register_map, 0, sizeof(register_map));
	register_map[MPU6050_REG_CHIP_ID] = MPU6050_CHIP_ID;
	register_map[MPU6050_REG_PWR_MGMT1] = MPU6050_SLEEP_EN;
	register_map[0x3F] = 0x40; /* ACCEL_ZOUT_H: high byte of 1g at ±2g FS */
	register_map[0x40] = 0x00; /* ACCEL_ZOUT_L */
	initialized = true;
}

int mpu6050_transfer(void *ctx, uint8_t reg, uint8_t *buf, size_t len, bool read)
{
	ARG_UNUSED(ctx);

	mpu6050_library_mock_init_once();

	if ((size_t)reg + len > sizeof(register_map)) {
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

	mpu6050_library_mock_init_once();

	return 0;
}
