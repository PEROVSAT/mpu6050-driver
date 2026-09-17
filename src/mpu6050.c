/*
 * Zephyr packaging: apply DT, bind the bus, call mpu6050_init().
 * Do not add device functions here. See include/mpu6050.h.
 */

#define DT_DRV_COMPAT invensense_mpu6050

#include "mpu6050_priv.h"

#if !defined(CONFIG_PEROVSAT_MPU6050_BACKEND_PUBLIC_MOCK)
	#include "mpu6050_bus.h"
#endif

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(mpu6050, CONFIG_LOG_DEFAULT_LEVEL);

mpu6050_t *mpu6050_from_dev(const struct device *dev)
{
	struct mpu6050_driver_data *data = dev->data;

	return &data->chip;
}

#if !defined(CONFIG_PEROVSAT_MPU6050_BACKEND_PUBLIC_MOCK)
void mpu6050_delay(uint32_t ms)
{
	k_msleep(ms);
}
#endif

static int mpu6050_driver_init(const struct device *dev)
{
	struct mpu6050_driver_data *data = dev->data;
	const struct mpu6050_driver_config *cfg = dev->config;

#if !defined(CONFIG_PEROVSAT_MPU6050_BACKEND_PUBLIC_MOCK)
	int ret;

	data->chip.bus_ctx = (void *)dev;

	ret = mpu6050_transfer_init(dev);
	if (ret < 0) {
		return ret;
	}
#endif

	return mpu6050_init(&data->chip, &cfg->chip);
}

#define MPU6050_INIT(inst)                                                                         \
	static struct mpu6050_driver_data mpu6050_data_##inst;                                     \
	static const struct mpu6050_driver_config mpu6050_config_##inst = {                        \
		.chip =                                                                            \
			{                                                                          \
				.accel_fs = DT_INST_PROP(inst, accel_fs),                          \
				.gyro_fs = DT_INST_PROP(inst, gyro_fs),                            \
				.smplrt_div = DT_INST_PROP(inst, smplrt_div),                      \
			},                                                                         \
	};                                                                                         \
	DEVICE_DT_INST_DEFINE(inst, mpu6050_driver_init, NULL, &mpu6050_data_##inst,               \
			      &mpu6050_config_##inst, BOOT_STAGE, BOOT_PRIORITY, NULL);

DT_INST_FOREACH_STATUS_OKAY(MPU6050_INIT)
