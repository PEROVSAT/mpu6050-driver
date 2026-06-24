/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mpu6050_lib.h"

#include <errno.h>

const uint16_t mpu6050_gyro_sensitivity_x10[4] = {1310, 655, 328, 164};

static int mpu6050_lib_transfer(mpu6050_transfer_fn transfer, void *ctx, uint8_t reg, uint8_t *buf,
				size_t len, bool read)
{
	if (transfer == NULL) {
		return -EINVAL;
	}

	return transfer(ctx, reg, buf, len, read);
}

static int mpu6050_lib_reg_read_byte(mpu6050_transfer_fn transfer, void *ctx, uint8_t reg,
				     uint8_t *val)
{
	return mpu6050_lib_transfer(transfer, ctx, reg, val, 1, true);
}

static int mpu6050_lib_reg_write_byte(mpu6050_transfer_fn transfer, void *ctx, uint8_t reg,
				      uint8_t val)
{
	return mpu6050_lib_transfer(transfer, ctx, reg, &val, 1, false);
}

static int mpu6050_lib_reg_update_byte(mpu6050_transfer_fn transfer, void *ctx, uint8_t reg,
				       uint8_t mask, uint8_t val)
{
	uint8_t old_val;
	int ret;

	ret = mpu6050_lib_reg_read_byte(transfer, ctx, reg, &old_val);
	if (ret < 0) {
		return ret;
	}

	return mpu6050_lib_reg_write_byte(transfer, ctx, reg, (old_val & ~mask) | (val & mask));
}

static int16_t mpu6050_be16_to_int16(const uint8_t *buf)
{
	return (int16_t)((uint16_t)buf[0] << 8 | buf[1]);
}

int mpu6050_lib_init(mpu6050_transfer_fn transfer, void *ctx, uint8_t accel_fs, uint16_t gyro_fs,
		     uint8_t smplrt_div, uint16_t *accel_sensitivity_shift,
		     uint16_t *gyro_sensitivity_x10, enum mpu6050_device_type *device_type)
{
	uint8_t id;
	uint8_t i;
	int ret;

	if (transfer == NULL || accel_sensitivity_shift == NULL || gyro_sensitivity_x10 == NULL ||
	    device_type == NULL) {
		return -EINVAL;
	}

	ret = mpu6050_lib_reg_read_byte(transfer, ctx, MPU6050_REG_CHIP_ID, &id);
	if (ret < 0) {
		return ret;
	}

	if (id == MPU6050_CHIP_ID || id == MPU9250_CHIP_ID || id == MPU6880_CHIP_ID) {
		*device_type = DEVICE_TYPE_MPU6050;
	} else if (id == MPU6500_CHIP_ID) {
		*device_type = DEVICE_TYPE_MPU6500;
	} else {
		return -EINVAL;
	}

	ret = mpu6050_lib_reg_update_byte(transfer, ctx, MPU6050_REG_PWR_MGMT1, MPU6050_SLEEP_EN,
					  0);
	if (ret < 0) {
		return ret;
	}

	for (i = 0U; i < 4U; i++) {
		if ((1U << (i + 1U)) == accel_fs) {
			break;
		}
	}

	if (i == 4U) {
		return -EINVAL;
	}

	ret = mpu6050_lib_reg_write_byte(transfer, ctx, MPU6050_REG_ACCEL_CFG,
					 i << MPU6050_ACCEL_FS_SHIFT);
	if (ret < 0) {
		return ret;
	}

	*accel_sensitivity_shift = 14U - i;

	for (i = 0U; i < 4U; i++) {
		if ((1U << i) * 250U == gyro_fs) {
			break;
		}
	}

	if (i == 4U) {
		return -EINVAL;
	}

	ret = mpu6050_lib_reg_write_byte(transfer, ctx, MPU6050_REG_GYRO_CFG,
					 i << MPU6050_GYRO_FS_SHIFT);
	if (ret < 0) {
		return ret;
	}

	*gyro_sensitivity_x10 = mpu6050_gyro_sensitivity_x10[i];

	ret = mpu6050_lib_reg_write_byte(transfer, ctx, MPU6050_REG_SMPLRT_DIV, smplrt_div);
	if (ret < 0) {
		return ret;
	}

	return 0;
}

int mpu6050_lib_sample_fetch(mpu6050_transfer_fn transfer, void *ctx, int16_t *accel_x,
			     int16_t *accel_y, int16_t *accel_z, int16_t *temp, int16_t *gyro_x,
			     int16_t *gyro_y, int16_t *gyro_z)
{
	uint8_t buf[14];
	int ret;

	if (transfer == NULL || accel_x == NULL || accel_y == NULL || accel_z == NULL ||
	    temp == NULL || gyro_x == NULL || gyro_y == NULL || gyro_z == NULL) {
		return -EINVAL;
	}

	ret = mpu6050_lib_transfer(transfer, ctx, MPU6050_REG_DATA_START, buf, sizeof(buf), true);
	if (ret < 0) {
		return ret;
	}

	*accel_x = mpu6050_be16_to_int16(&buf[0]);
	*accel_y = mpu6050_be16_to_int16(&buf[2]);
	*accel_z = mpu6050_be16_to_int16(&buf[4]);
	*temp = mpu6050_be16_to_int16(&buf[6]);
	*gyro_x = mpu6050_be16_to_int16(&buf[8]);
	*gyro_y = mpu6050_be16_to_int16(&buf[10]);
	*gyro_z = mpu6050_be16_to_int16(&buf[12]);

	return 0;
}
