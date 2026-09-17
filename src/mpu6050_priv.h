#pragma once

/*
 * Zephyr packaging. Do not add device functions here.
 */

#define BOOT_STAGE    POST_KERNEL
#define BOOT_PRIORITY 100

#include <zephyr/device.h>

#include <mpu6050.h>

struct mpu6050_driver_config {
	mpu6050_config_t chip;

	/* FILL IN when bringing up hardware, e.g. struct i2c_dt_spec bus; */
};

struct mpu6050_driver_data {
	mpu6050_t chip;
};

#if !defined(CONFIG_PEROVSAT_MPU6050_BACKEND_PUBLIC_MOCK)
/* Defined in hardware_transfer.c or lib_mock_transfer.c. */
int mpu6050_transfer_init(const struct device *dev);
#endif
