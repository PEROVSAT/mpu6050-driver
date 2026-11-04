#include "mpu6050.h"
#include "mpu6050_bus.h"
#include "mpu6050_regs.h"

#include <errno.h>

#define MPU6050_ACCEL_MPS2_PER_G_MICRO 9806650LL
#define MPU6050_PI_MICRO               3141593LL
#define MPU6050_DEGREES_PER_CIRCLE     180LL

/* measured in degrees/sec x10 to avoid floating point */
static const uint16_t mpu6050_gyro_sensitivity_x10[4] = {1310, 655, 328, 164};

static int mpu6050_reg_read_byte(mpu6050_t *dev, uint8_t reg, uint8_t *val)
{
	return mpu6050_transfer(dev->bus_ctx, reg, val, 1, true);
}

static int mpu6050_reg_write_byte(mpu6050_t *dev, uint8_t reg, uint8_t val)
{
	return mpu6050_transfer(dev->bus_ctx, reg, &val, 1, false);
}

static int mpu6050_reg_update_byte(mpu6050_t *dev, uint8_t reg, uint8_t mask, uint8_t val)
{
	uint8_t old_val;
	int ret;

	ret = mpu6050_reg_read_byte(dev, reg, &old_val);
	if (ret < 0) {
		return ret;
	}

	return mpu6050_reg_write_byte(dev, reg, (old_val & ~mask) | (val & mask));
}

static int16_t mpu6050_be16_to_int16(const uint8_t *buf)
{
	return (int16_t)((uint16_t)buf[0] << 8 | buf[1]);
}

static int mpu6050_set_sensor_value(int64_t total_micro, psp_sensor_value *out)
{
	if (out == NULL) {
		return -EINVAL;
	}

	out->whole = (int32_t)(total_micro / 1000000LL);
	out->micro = (int32_t)(total_micro % 1000000LL);

	return 0;
}

static int mpu6050_convert_gyro_axis(const mpu6050_t *dev, int16_t raw, psp_sensor_value *out)
{
	int64_t total_micro;

	if (dev == NULL || out == NULL || dev->gyro_sensitivity_x10 == 0U) {
		return -EINVAL;
	}

	/*
	 * gyro_sensitivity_x10 stores counts per degree/second multiplied by 10.
	 *
	 * raw * 10 / sensitivity_x10 = degrees/second
	 * degrees/second * pi / 180 = radians/second
	 *
	 * The result is calculated directly in millionths of radian/second.
	 */
	total_micro = ((int64_t)raw * 10LL * MPU6050_PI_MICRO) /
		      ((int64_t)dev->gyro_sensitivity_x10 * MPU6050_DEGREES_PER_CIRCLE);

	return mpu6050_set_sensor_value(total_micro, out);
}

static int mpu6050_convert_accel_axis(const mpu6050_t *dev, int16_t raw, psp_sensor_value *out)
{
	int64_t total_micro;

	if (dev == NULL || out == NULL || dev->accel_sensitivity_shift >= 63U) {
		return -EINVAL;
	}

	const int64_t counts_per_g = (int64_t)1 << dev->accel_sensitivity_shift;
	if (counts_per_g == 0) {
		return -EINVAL;
	}

	/*
	 * raw / counts_per_g = g
	 * g * 9.80665 = m/s^2
	 *
	 * MPU6050_ACCEL_MPS2_PER_G_MICRO is 9.80665 * 1,000,000.
	 */
	total_micro = ((int64_t)raw * MPU6050_ACCEL_MPS2_PER_G_MICRO) / counts_per_g;

	return mpu6050_set_sensor_value(total_micro, out);
}
int mpu6050_init(mpu6050_t *dev, const mpu6050_config_t *cfg)
{
	uint8_t id;
	uint8_t i;
	int ret;

	if (dev == NULL || cfg == NULL) {
		return -EINVAL;
	}

	ret = mpu6050_reg_read_byte(dev, MPU6050_REG_CHIP_ID, &id);
	if (ret < 0) {
		return ret;
	}

	if (id == MPU6050_CHIP_ID || id == MPU9250_CHIP_ID || id == MPU6880_CHIP_ID) {
		dev->device_type = MPU6050_DEVICE_TYPE_MPU6050;
	} else if (id == MPU6500_CHIP_ID) {
		dev->device_type = MPU6050_DEVICE_TYPE_MPU6500;
	} else {
		return -EINVAL;
	}

	ret = mpu6050_reg_update_byte(dev, MPU6050_REG_PWR_MGMT1, MPU6050_SLEEP_EN, 0);
	if (ret < 0) {
		return ret;
	}

	for (i = 0U; i < 4U; i++) {
		if ((1U << (i + 1U)) == cfg->accel_fs) {
			break;
		}
	}

	if (i == 4U) {
		return -EINVAL;
	}

	ret = mpu6050_reg_write_byte(dev, MPU6050_REG_ACCEL_CFG, i << MPU6050_ACCEL_FS_SHIFT);
	if (ret < 0) {
		return ret;
	}

	dev->accel_sensitivity_shift = 14U - i;

	for (i = 0U; i < 4U; i++) {
		if ((1U << i) * 250U == cfg->gyro_fs) {
			break;
		}
	}

	if (i == 4U) {
		return -EINVAL;
	}

	ret = mpu6050_reg_write_byte(dev, MPU6050_REG_GYRO_CFG, i << MPU6050_GYRO_FS_SHIFT);
	if (ret < 0) {
		return ret;
	}

	dev->gyro_sensitivity_x10 = mpu6050_gyro_sensitivity_x10[i];

	ret = mpu6050_reg_write_byte(dev, MPU6050_REG_SMPLRT_DIV, cfg->smplrt_div);
	if (ret < 0) {
		return ret;
	}

	return 0;
}

int mpu6050_sample_fetch(mpu6050_t *dev, psp_imu_reading *reading)
{
	uint8_t buf[14];
	int ret;

	if (dev == NULL || reading == NULL) {
		return -EINVAL;
	}

	ret = mpu6050_transfer(dev->bus_ctx, MPU6050_REG_DATA_START, buf, sizeof(buf), true);
	if (ret < 0) {
		return ret;
	}

	dev->accel_x = mpu6050_be16_to_int16(&buf[0]);
	dev->accel_y = mpu6050_be16_to_int16(&buf[2]);
	dev->accel_z = mpu6050_be16_to_int16(&buf[4]);
	dev->temp = mpu6050_be16_to_int16(&buf[6]);
	dev->gyro_x = mpu6050_be16_to_int16(&buf[8]);
	dev->gyro_y = mpu6050_be16_to_int16(&buf[10]);
	dev->gyro_z = mpu6050_be16_to_int16(&buf[12]);

	ret = mpu6050_convert_gyro_axis(dev, dev->gyro_x, &reading->gyro[0]);
	if (ret < 0) {
		return ret;
	}

	ret = mpu6050_convert_gyro_axis(dev, dev->gyro_y, &reading->gyro[1]);
	if (ret < 0) {
		return ret;
	}

	ret = mpu6050_convert_gyro_axis(dev, dev->gyro_z, &reading->gyro[2]);
	if (ret < 0) {
		return ret;
	}

	ret = mpu6050_convert_accel_axis(dev, dev->accel_x, &reading->accel[0]);
	if (ret < 0) {
		return ret;
	}

	ret = mpu6050_convert_accel_axis(dev, dev->accel_y, &reading->accel[1]);
	if (ret < 0) {
		return ret;
	}

	ret = mpu6050_convert_accel_axis(dev, dev->accel_z, &reading->accel[2]);
	if (ret < 0) {
		return ret;
	}

	return 0;
}
