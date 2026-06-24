/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MPU6050_REG_CHIP_ID 0x75
#define MPU6050_CHIP_ID     0x68
#define MPU6500_CHIP_ID     0x70
#define MPU9250_CHIP_ID     0x71
#define MPU6880_CHIP_ID     0x19

#define MPU6050_REG_SMPLRT_DIV 0x19

#define MPU6050_REG_GYRO_CFG  0x1B
#define MPU6050_GYRO_FS_SHIFT 3

#define MPU6050_REG_ACCEL_CFG  0x1C
#define MPU6050_ACCEL_FS_SHIFT 3

#define MPU6050_REG_DATA_START 0x3B

#define MPU6050_REG_PWR_MGMT1 0x6B
#define MPU6050_SLEEP_EN      (1U << 6)

/* measured in degrees/sec x10 to avoid floating point */
extern const uint16_t mpu6050_gyro_sensitivity_x10[4];

/** Device type, uses the correct offsets for a particular device */
enum mpu6050_device_type {
	DEVICE_TYPE_MPU6050 = 0,
	DEVICE_TYPE_MPU6500,
};

/**
 * Bus-agnostic byte transfer callback.
 *
 * @param ctx  Opaque context (typically the Zephyr device pointer).
 * @param reg  Register address or protocol offset.
 * @param buf  Data buffer.
 * @param len  Length of @p buf.
 * @param read true for read, false for write.
 *
 * @return 0 on success, negative errno on failure.
 */
typedef int (*mpu6050_transfer_fn)(void *ctx, uint8_t reg, uint8_t *buf, size_t len, bool read);

/**
 * Initialize the device library with an injected transfer function.
 *
 * @param transfer               Bus transfer callback.
 * @param ctx                    Opaque context passed to @p transfer.
 * @param accel_fs               Accelerometer full-scale range (2, 4, 8, or 16 g).
 * @param gyro_fs                Gyroscope full-scale range (250, 500, 1000, or 2000 dps).
 * @param smplrt_div             Sample rate divider.
 * @param accel_sensitivity_shift Output: accelerometer sensitivity shift.
 * @param gyro_sensitivity_x10   Output: gyroscope sensitivity (degrees/sec x10).
 * @param device_type            Output: detected device type.
 *
 * @return 0 on success, negative errno on failure.
 */
int mpu6050_lib_init(mpu6050_transfer_fn transfer, void *ctx, uint8_t accel_fs, uint16_t gyro_fs,
		     uint8_t smplrt_div, uint16_t *accel_sensitivity_shift,
		     uint16_t *gyro_sensitivity_x10, enum mpu6050_device_type *device_type);

/**
 * Fetch a new sensor sample.
 *
 * @param transfer Bus transfer callback.
 * @param ctx      Opaque context passed to @p transfer.
 * @param accel_x  Output: raw accelerometer X.
 * @param accel_y  Output: raw accelerometer Y.
 * @param accel_z  Output: raw accelerometer Z.
 * @param temp     Output: raw temperature.
 * @param gyro_x   Output: raw gyroscope X.
 * @param gyro_y   Output: raw gyroscope Y.
 * @param gyro_z   Output: raw gyroscope Z.
 *
 * @return 0 on success, negative errno on failure.
 */
int mpu6050_lib_sample_fetch(mpu6050_transfer_fn transfer, void *ctx, int16_t *accel_x,
			     int16_t *accel_y, int16_t *accel_z, int16_t *temp, int16_t *gyro_x,
			     int16_t *gyro_y, int16_t *gyro_z);
