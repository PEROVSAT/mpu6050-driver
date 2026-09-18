/*
 * Public fake of the device API. Same symbols as lib/mpu6050.c;
 * CMake links exactly one of the two.
 */

#include "mpu6050.h"

#include <stddef.h>

int mpu6050_init(mpu6050_t *dev, const mpu6050_config_t *cfg)
{
	(void)dev;
	(void)cfg;

	return 0;
}

int mpu6050_sample_fetch(mpu6050_t *dev)
{
	if (dev == NULL) {
		return 0;
	}

	dev->accel_x = 100;
	dev->accel_y = 200;
	dev->accel_z = 16384;
	dev->gyro_x = 10;
	dev->gyro_y = 20;
	dev->gyro_z = 30;
	dev->temp = 25;

	return 0;
}
