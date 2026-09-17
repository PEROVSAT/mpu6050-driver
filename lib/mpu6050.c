#include "mpu6050.h"
#include "mpu6050_bus.h"
#include "mpu6050_regs.h"

#include <errno.h>

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

int mpu6050_sample_fetch(mpu6050_t *dev)
{
	uint8_t buf[14];
	int ret;

	if (dev == NULL) {
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

	return 0;
}
