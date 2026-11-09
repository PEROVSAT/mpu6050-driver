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
	psp_imu_reading reading = {0};

	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	if (dev == NULL) {
		return -ENODEV;
	}

	ret = mpu6050_sample_fetch(dev, &reading);
	if (ret < 0) {
		shell_error(sh, "error %d", ret);
		return ret;
	}

	if (dev->device_type == MPU6050_DEVICE_TYPE_MPU6500) {
		temp_c = (float)dev->temp / 333.87f + 21.0f;
	} else {
		temp_c = (float)dev->temp / 340.0f + 36.53f;
	}

	shell_print(sh, "accel [m/s^2] %.3f %.3f %.3f", reading.accel[0].whole,
		    reading.accel[0].micro,                          // X
		    reading.accel[1].whole, reading.accel[1].micro,  // Y
		    reading.accel[2].whole, reading.accel[2].micro); // Z
	shell_print(sh, "gyro [deg/s] %.1f %.1f %.1f", reading.gyro[0].whole,
		    reading.gyro[0].micro,                         // X
		    reading.gyro[1].whole, reading.gyro[1].micro,  // Y
		    reading.gyro[2].whole, reading.gyro[2].micro); // Z
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(mpu6050_cmds, SHELL_CMD(fetch, NULL, "Fetch one sample", cmd_fetch),
			       SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(mpu6050, &mpu6050_cmds, "MPU6050 driver", NULL);

int main(void)
{
	return 0;
}
