/*
 * Public fake of the device API. Same symbols as lib/mpu6050.c;
 * CMake links exactly one of the two.
 */

#include "mpu6050.h"
#include <errno.h>
#include <stddef.h>

int mpu6050_init(mpu6050_t *dev, const mpu6050_config_t *cfg)
{
	if (dev == NULL || cfg == NULL) {
		return -EINVAL;
	}
	(void)dev;
	(void)cfg;

	return 0;
}

int mpu6050_sample_fetch(mpu6050_t *dev, psp_imu_reading *reading)
{
	if (dev == NULL || reading == NULL) {
		return -EINVAL;
	}

	// Based on the psp_sensor_value struct {whole, micro}
	reading->gyro[0] = (psp_sensor_value){0, 100000};  // 0.1
	reading->gyro[1] = (psp_sensor_value){0, 200000};  // 0.2
	reading->gyro[2] = (psp_sensor_value){0, 300000};  // 0.3
	reading->accel[0] = (psp_sensor_value){0, 100000}; // 0.1
	reading->accel[1] = (psp_sensor_value){9, 800000}; // 9.8
	reading->accel[2] = (psp_sensor_value){0, 500000}; // 0.5

	return 0;
}
