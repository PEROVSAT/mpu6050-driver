#pragma once

#define BOOT_STAGE    POST_KERNEL
#define BOOT_PRIORITY 100

#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#if defined(CONFIG_PEROVSAT_MPU6050_BACKEND_HARDWARE)
#include <zephyr/drivers/i2c.h>
#endif

#if !defined(CONFIG_PEROVSAT_MPU6050_BACKEND_PUBLIC_MOCK)
#include "mpu6050_lib.h"
#endif

/** Read-only configuration, populated from devicetree. */
struct mpu6050_config {
	uint8_t accel_fs;
	uint16_t gyro_fs;
	uint8_t smplrt_div;

#if defined(CONFIG_PEROVSAT_MPU6050_BACKEND_HARDWARE)
	struct i2c_dt_spec bus;
#endif
};

/** Mutable runtime state. */
struct mpu6050_data {
#if !defined(CONFIG_PEROVSAT_MPU6050_BACKEND_PUBLIC_MOCK)
	int16_t accel_x;
	int16_t accel_y;
	int16_t accel_z;
	int16_t temp;
	int16_t gyro_x;
	int16_t gyro_y;
	int16_t gyro_z;
	uint16_t accel_sensitivity_shift;
	uint16_t gyro_sensitivity_x10;
	enum mpu6050_device_type device_type;
#endif

#if defined(CONFIG_PEROVSAT_MPU6050_BACKEND_SIMULATION)
	/* Define things like a socket fd */
#endif
};
