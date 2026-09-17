/*
 * Public fake of the device API. Same symbols as lib/mpu6050.c;
 * CMake links exactly one of the two.
 */

#include "mpu6050.h"

int mpu6050_init(mpu6050_t *dev, const mpu6050_config_t *cfg)
{
	(void)dev;
	(void)cfg;

	return 0;
}

int mpu6050_sample_fetch(mpu6050_t *dev)
{
	(void)dev;

	return 0;
}
