#include <zephyr/device.h>
#include <zephyr/shell/shell.h>

#include <mpu6050.h>

static mpu6050_t *sh_dev(const struct shell *sh)
{
	const struct device *zdev = DEVICE_DT_GET(DT_ALIAS(mpu6050));

	if (!device_is_ready(zdev)) {
		shell_error(sh, "device not ready");
		return NULL;
	}

	return mpu6050_from_dev(zdev);
}

static int cmd_fetch(const struct shell *sh, size_t argc, char **argv)
{
	mpu6050_t *dev = sh_dev(sh);
	float accel_scale;
	float gyro_scale;
	float temp_c;
	int ret;

	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	if (dev == NULL) {
		return -ENODEV;
	}

	ret = mpu6050_sample_fetch(dev);
	if (ret < 0) {
		shell_error(sh, "error %d", ret);
		return ret;
	}

	accel_scale = -1.0f / (float)(1U << dev->accel_sensitivity_shift);
	gyro_scale = 10.0f / (float)dev->gyro_sensitivity_x10;
	if (dev->device_type == MPU6050_DEVICE_TYPE_MPU6500) {
		temp_c = (float)dev->temp / 333.87f + 21.0f;
	} else {
		temp_c = (float)dev->temp / 340.0f + 36.53f;
	}

	shell_print(sh, "accel [g] %.3f %.3f %.3f", dev->accel_x * accel_scale,
		    dev->accel_y * accel_scale, dev->accel_z * accel_scale);
	shell_print(sh, "gyro [deg/s] %.1f %.1f %.1f", dev->gyro_x * gyro_scale,
		    dev->gyro_y * gyro_scale, dev->gyro_z * gyro_scale);
	shell_print(sh, "temp [deg C] %.2f", temp_c);
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(mpu6050_cmds, SHELL_CMD(fetch, NULL, "Fetch one sample", cmd_fetch),
			       SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(mpu6050, &mpu6050_cmds, "MPU6050 driver", NULL);

int main(void)
{
	return 0;
}
