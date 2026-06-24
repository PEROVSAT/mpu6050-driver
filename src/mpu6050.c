/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT zephyr_mpu6050

#include "mpu6050.h"

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#if !defined(CONFIG_PEROVSAT_MPU6050_BACKEND_PUBLIC_MOCK)
#include "transfer.h"
#endif

LOG_MODULE_REGISTER(mpu6050, CONFIG_LOG_DEFAULT_LEVEL);

#if !defined(CONFIG_PEROVSAT_MPU6050_BACKEND_PUBLIC_MOCK)

/* see "Accelerometer Measurements" section from register map description */
static void mpu6050_convert_accel(struct sensor_value *val, int16_t raw_val,
				  uint16_t sensitivity_shift)
{
	int64_t conv_val;

	conv_val = ((int64_t)raw_val * SENSOR_G) >> sensitivity_shift;
	val->val1 = conv_val / 1000000;
	val->val2 = conv_val % 1000000;
}

/* see "Gyroscope Measurements" section from register map description */
static void mpu6050_convert_gyro(struct sensor_value *val, int16_t raw_val,
				 uint16_t sensitivity_x10)
{
	int64_t conv_val;

	conv_val = ((int64_t)raw_val * SENSOR_PI * 10) / (sensitivity_x10 * 180U);
	val->val1 = conv_val / 1000000;
	val->val2 = conv_val % 1000000;
}

/* see "Temperature Measurement" section from register map description */
static void mpu6050_convert_temp(enum mpu6050_device_type device_type, struct sensor_value *val,
				 int16_t raw_val)
{
	int64_t tmp_val = (int64_t)raw_val * 1000000;

	switch (device_type) {
	case DEVICE_TYPE_MPU6500:
		tmp_val = (tmp_val * 1000 / 333870) + 21000000;
		break;

	case DEVICE_TYPE_MPU6050:
	default:
		tmp_val = (tmp_val / 340) + 36000000;
		break;
	}

	val->val1 = tmp_val / 1000000;
	val->val2 = tmp_val % 1000000;
}

static int sample_fetch(const struct device *dev, enum sensor_channel channel)
{
	struct mpu6050_data *data = dev->data;

	ARG_UNUSED(channel);

	return mpu6050_lib_sample_fetch(mpu6050_transfer, (void *)dev, &data->accel_x,
					&data->accel_y, &data->accel_z, &data->temp, &data->gyro_x,
					&data->gyro_y, &data->gyro_z);
}

static int channel_get(const struct device *dev, enum sensor_channel channel,
		       struct sensor_value *val)
{
	struct mpu6050_data *data = dev->data;

	switch (channel) {
	case SENSOR_CHAN_ACCEL_XYZ:
		mpu6050_convert_accel(val, data->accel_x, data->accel_sensitivity_shift);
		mpu6050_convert_accel(val + 1, data->accel_y, data->accel_sensitivity_shift);
		mpu6050_convert_accel(val + 2, data->accel_z, data->accel_sensitivity_shift);
		break;
	case SENSOR_CHAN_ACCEL_X:
		mpu6050_convert_accel(val, data->accel_x, data->accel_sensitivity_shift);
		break;
	case SENSOR_CHAN_ACCEL_Y:
		mpu6050_convert_accel(val, data->accel_y, data->accel_sensitivity_shift);
		break;
	case SENSOR_CHAN_ACCEL_Z:
		mpu6050_convert_accel(val, data->accel_z, data->accel_sensitivity_shift);
		break;
	case SENSOR_CHAN_GYRO_XYZ:
		mpu6050_convert_gyro(val, data->gyro_x, data->gyro_sensitivity_x10);
		mpu6050_convert_gyro(val + 1, data->gyro_y, data->gyro_sensitivity_x10);
		mpu6050_convert_gyro(val + 2, data->gyro_z, data->gyro_sensitivity_x10);
		break;
	case SENSOR_CHAN_GYRO_X:
		mpu6050_convert_gyro(val, data->gyro_x, data->gyro_sensitivity_x10);
		break;
	case SENSOR_CHAN_GYRO_Y:
		mpu6050_convert_gyro(val, data->gyro_y, data->gyro_sensitivity_x10);
		break;
	case SENSOR_CHAN_GYRO_Z:
		mpu6050_convert_gyro(val, data->gyro_z, data->gyro_sensitivity_x10);
		break;
	case SENSOR_CHAN_DIE_TEMP:
		mpu6050_convert_temp(data->device_type, val, data->temp);
		break;
	default:
		return -ENOTSUP;
	}

	return 0;
}

#else /* CONFIG_PEROVSAT_MPU6050_BACKEND_PUBLIC_MOCK */

static int sample_fetch(const struct device *dev, enum sensor_channel channel)
{
	ARG_UNUSED(dev);

	if (channel != SENSOR_CHAN_ALL && channel != SENSOR_CHAN_ACCEL_XYZ &&
	    channel != SENSOR_CHAN_GYRO_XYZ) {
		return -ENOTSUP;
	}

	return 0;
}

static int channel_get(const struct device *dev, enum sensor_channel channel,
		       struct sensor_value *val)
{
	ARG_UNUSED(dev);

	if (channel == SENSOR_CHAN_ACCEL_XYZ) {
		sensor_value_from_double(&val[0], 0.0);
		sensor_value_from_double(&val[1], 0.0);
		sensor_value_from_double(&val[2], 9.806);
		return 0;
	}

	if (channel == SENSOR_CHAN_GYRO_XYZ) {
		sensor_value_from_double(&val[0], 0.0);
		sensor_value_from_double(&val[1], 0.0);
		sensor_value_from_double(&val[2], 0.0);
		return 0;
	}

	return -ENOTSUP;
}

#endif /* CONFIG_PEROVSAT_MPU6050_BACKEND_PUBLIC_MOCK */

const struct sensor_driver_api mpu6050_api = {
	.sample_fetch = sample_fetch,
	.channel_get = channel_get,
};

static int mpu6050_init(const struct device *dev)
{
#if !defined(CONFIG_PEROVSAT_MPU6050_BACKEND_PUBLIC_MOCK)
	const struct mpu6050_config *config = dev->config;
	struct mpu6050_data *data = dev->data;
	int ret;

	ret = mpu6050_transfer_init(dev);
	if (ret < 0) {
		return ret;
	}

	return mpu6050_lib_init(mpu6050_transfer, (void *)dev, config->accel_fs, config->gyro_fs,
				config->smplrt_div, &data->accel_sensitivity_shift,
				&data->gyro_sensitivity_x10, &data->device_type);
#else
	ARG_UNUSED(dev);
	return 0;
#endif
}

#define MPU6050_INIT(inst)                                                                         \
	static struct mpu6050_data mpu6050_data_##inst;                                            \
	static const struct mpu6050_config mpu6050_config_##inst = {                               \
		.accel_fs = DT_INST_PROP(inst, accel_fs),                                          \
		.gyro_fs = DT_INST_PROP(inst, gyro_fs),                                            \
		.smplrt_div = DT_INST_PROP(inst, smplrt_div),                                      \
		IF_ENABLED(CONFIG_PEROVSAT_MPU6050_BACKEND_HARDWARE,                                 \
			   (.bus = I2C_DT_SPEC_INST_GET(inst),)) };                                  \
	SENSOR_DEVICE_DT_INST_DEFINE(inst, mpu6050_init, NULL, &mpu6050_data_##inst,               \
				     &mpu6050_config_##inst, BOOT_STAGE, BOOT_PRIORITY,            \
				     &mpu6050_api);

DT_INST_FOREACH_STATUS_OKAY(MPU6050_INIT)
