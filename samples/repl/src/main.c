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

	shell_print(sh, "accel %d %d %d", dev->accel_x, dev->accel_y, dev->accel_z);
	shell_print(sh, "gyro %d %d %d", dev->gyro_x, dev->gyro_y, dev->gyro_z);
	shell_print(sh, "temp %d", dev->temp);
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(mpu6050_cmds, SHELL_CMD(fetch, NULL, "Fetch one sample", cmd_fetch),
			       SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(mpu6050, &mpu6050_cmds, "MPU6050 driver", NULL);

int main(void)
{
	return 0;
}
