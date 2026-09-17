/*
 * Unit tests for the portable device library.
 *
 * Links lib/mpu6050.c only. Do not use src/. When a test talks to
 * the bus, define mpu6050_transfer and mpu6050_delay in
 * this file (see lib/mpu6050_bus.h).
 */

#include <errno.h>
#include <string.h>

#include <zephyr/ztest.h>

#include "mpu6050.h"
#include "mpu6050_bus.h"
#include "mpu6050_regs.h"

static uint8_t fake_register_map[256];

int mpu6050_transfer(void *ctx, uint8_t reg, uint8_t *buf, size_t len, bool read)
{
	ARG_UNUSED(ctx);

	if ((size_t)reg + len > sizeof(fake_register_map)) {
		return -EINVAL;
	}

	if (read) {
		memcpy(buf, &fake_register_map[reg], len);
	} else {
		memcpy(&fake_register_map[reg], buf, len);
	}

	return 0;
}

void mpu6050_delay(uint32_t ms)
{
	ARG_UNUSED(ms);
}

static void reset_map(void)
{
	memset(fake_register_map, 0, sizeof(fake_register_map));
	fake_register_map[MPU6050_REG_CHIP_ID] = MPU6050_CHIP_ID;
	fake_register_map[MPU6050_REG_PWR_MGMT1] = MPU6050_SLEEP_EN;
}

ZTEST(mpu6050_unit, test_init_rejects_null)
{
	mpu6050_t dev = {
		.bus_ctx = (void *)1,
	};
	mpu6050_config_t cfg = {
		.accel_fs = 2,
		.gyro_fs = 250,
		.smplrt_div = 0,
	};

	zassert_equal(mpu6050_init(NULL, &cfg), -EINVAL);
	zassert_equal(mpu6050_init(&dev, NULL), -EINVAL);
}

ZTEST(mpu6050_unit, test_init_configures_device)
{
	mpu6050_t dev = {
		.bus_ctx = (void *)1,
	};
	mpu6050_config_t cfg = {
		.accel_fs = 2,
		.gyro_fs = 250,
		.smplrt_div = 9,
	};

	reset_map();

	zassert_ok(mpu6050_init(&dev, &cfg));
	zassert_equal(dev.device_type, MPU6050_DEVICE_TYPE_MPU6050);
	zassert_equal(dev.accel_sensitivity_shift, 14);
	zassert_equal(dev.gyro_sensitivity_x10, 1310);
	zassert_equal(fake_register_map[MPU6050_REG_PWR_MGMT1] & MPU6050_SLEEP_EN, 0);
	zassert_equal(fake_register_map[MPU6050_REG_ACCEL_CFG], 0);
	zassert_equal(fake_register_map[MPU6050_REG_GYRO_CFG], 0);
	zassert_equal(fake_register_map[MPU6050_REG_SMPLRT_DIV], 9);
}

ZTEST(mpu6050_unit, test_init_rejects_unknown_chip)
{
	mpu6050_t dev = {
		.bus_ctx = (void *)1,
	};
	mpu6050_config_t cfg = {
		.accel_fs = 2,
		.gyro_fs = 250,
	};

	reset_map();
	fake_register_map[MPU6050_REG_CHIP_ID] = 0x00;

	zassert_equal(mpu6050_init(&dev, &cfg), -EINVAL);
}

ZTEST(mpu6050_unit, test_init_mpu6500)
{
	mpu6050_t dev = {
		.bus_ctx = (void *)1,
	};
	mpu6050_config_t cfg = {
		.accel_fs = 8,
		.gyro_fs = 1000,
		.smplrt_div = 0,
	};

	reset_map();
	fake_register_map[MPU6050_REG_CHIP_ID] = MPU6500_CHIP_ID;

	zassert_ok(mpu6050_init(&dev, &cfg));
	zassert_equal(dev.device_type, MPU6050_DEVICE_TYPE_MPU6500);
	zassert_equal(dev.accel_sensitivity_shift, 12);
	zassert_equal(dev.gyro_sensitivity_x10, 328);
	zassert_equal(fake_register_map[MPU6050_REG_ACCEL_CFG], 2 << MPU6050_ACCEL_FS_SHIFT);
	zassert_equal(fake_register_map[MPU6050_REG_GYRO_CFG], 2 << MPU6050_GYRO_FS_SHIFT);
}

ZTEST(mpu6050_unit, test_sample_fetch)
{
	mpu6050_t dev = {
		.bus_ctx = (void *)1,
	};

	reset_map();
	fake_register_map[0x3B] = 0x00;
	fake_register_map[0x3C] = 0x01;
	fake_register_map[0x3D] = 0x00;
	fake_register_map[0x3E] = 0x02;
	fake_register_map[0x3F] = 0x40;
	fake_register_map[0x40] = 0x00;
	fake_register_map[0x41] = 0x00;
	fake_register_map[0x42] = 0x03;
	fake_register_map[0x43] = 0x00;
	fake_register_map[0x44] = 0x04;
	fake_register_map[0x45] = 0x00;
	fake_register_map[0x46] = 0x05;
	fake_register_map[0x47] = 0x00;
	fake_register_map[0x48] = 0x06;

	zassert_ok(mpu6050_sample_fetch(&dev));
	zassert_equal(dev.accel_x, 1);
	zassert_equal(dev.accel_y, 2);
	zassert_equal(dev.accel_z, 0x4000);
	zassert_equal(dev.temp, 3);
	zassert_equal(dev.gyro_x, 4);
	zassert_equal(dev.gyro_y, 5);
	zassert_equal(dev.gyro_z, 6);
}

ZTEST_SUITE(mpu6050_unit, NULL, NULL, NULL, NULL, NULL);
