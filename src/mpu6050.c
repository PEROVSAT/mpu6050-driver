#define DT_DRV_COMPAT invensense_mpu6050

#include "mpu6050.h"

#include <zephyr/device.h>
#include <zephyr/logging/log.h>

#if !defined(CONFIG_PEROVSAT_MPU6050_BACKEND_PUBLIC_MOCK)
#include "transfer.h"
#endif

LOG_MODULE_REGISTER(mpu6050, CONFIG_LOG_DEFAULT_LEVEL);

// Static API functions that are defined in mpu6050.h get implemented here

/*
static int read_sensor(const struct device *dev, uint8_t *val)
{
	#if !defined(CONFIG_PEROVSAT_MPU6050_BACKEND_PUBLIC_MOCK)
		return mpu6050_lib_read_sensor(mpu6050_transfer, dev, val);
	#else
		ARG_UNUSED(dev);
		*val = 0x01;
		return 0;
	#endif
}
*/

const struct mpu6050_driver_api mpu6050_api = {
	// .read_sensor = read_sensor,
};

static int mpu6050_init(const struct device *dev)
{
#if !defined(CONFIG_PEROVSAT_MPU6050_BACKEND_PUBLIC_MOCK)
	// Initialize transfer backend and library
	int ret = mpu6050_transfer_init(dev);
	if (ret < 0) {
		return ret;
	}

	return mpu6050_lib_init(mpu6050_transfer, dev);
#else
	ARG_UNUSED(dev);
	return 0;
#endif
}

#define MPU6050_INIT(inst)                                                                         \
	static struct mpu6050_data mpu6050_data_##inst;                                            \
	static const struct mpu6050_config mpu6050_config_##inst = {                               \
		/* FILL IN: .bus = I2C_DT_SPEC_INST_GET(inst), */                                  \
	};                                                                                         \
	DEVICE_DT_INST_DEFINE(inst, mpu6050_init, NULL, &mpu6050_data_##inst,                      \
			      &mpu6050_config_##inst, BOOT_STAGE, BOOT_PRIORITY, &mpu6050_api);

DT_INST_FOREACH_STATUS_OKAY(MPU6050_INIT)
