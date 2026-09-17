#if 0
	#include <zephyr/device.h>
	#include <zephyr/shell/shell.h>

	#include <mpu6050.h>

static int cmd_init(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	const struct device *zdev = DEVICE_DT_GET(DT_ALIAS(mpu6050));

	if (!device_is_ready(zdev)) {
		shell_error(sh, "device not ready");
		return -ENODEV;
	}

	/* FILL IN: call device functions via mpu6050_from_dev(zdev) */

	shell_print(sh, "ok");
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(mpu6050_cmds,
			       SHELL_CMD(init, NULL, "Placeholder", cmd_init),
			       SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(mpu6050, &mpu6050_cmds, "MPU6050", NULL);
#endif

int main(void)
{
	return 0;
}
