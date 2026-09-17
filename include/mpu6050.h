#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Device API. Adding a function:
 *   1. Declare it here
 *   2. Implement it in lib/mpu6050.c
 *   3. Stub it in mock/mpu6050.c
 *
 * Do not edit src/ for API work.
 */

typedef enum {
	MPU6050_DEVICE_TYPE_MPU6050 = 0,
	MPU6050_DEVICE_TYPE_MPU6500,
} mpu6050_device_type_t;

typedef struct {
	uint8_t accel_fs;
	uint16_t gyro_fs;
	uint8_t smplrt_div;
} mpu6050_config_t;

typedef struct mpu6050 {
	/*
	 * Which instance to talk to. src/ sets this to the Zephyr device
	 * pointer; unit tests set it to any non-NULL token. App code should
	 * not read or write it.
	 *
	 * The bus implementation is a named function (mpu6050_transfer),
	 * not a pointer on this object. See lib/mpu6050_bus.h.
	 */
	void *bus_ctx;

	uint16_t accel_sensitivity_shift;
	uint16_t gyro_sensitivity_x10;
	mpu6050_device_type_t device_type;

	int16_t accel_x;
	int16_t accel_y;
	int16_t accel_z;
	int16_t temp;
	int16_t gyro_x;
	int16_t gyro_y;
	int16_t gyro_z;
} mpu6050_t;

int mpu6050_init(mpu6050_t *dev, const mpu6050_config_t *cfg);
int mpu6050_sample_fetch(mpu6050_t *dev);

/* App glue: Zephyr `struct device` → this object. Implemented in src/. */
struct device;
mpu6050_t *mpu6050_from_dev(const struct device *dev);

#ifdef __cplusplus
}
#endif
