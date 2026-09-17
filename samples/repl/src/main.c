#if 0
#include <zephyr/device.h>
#include <zephyr/shell/shell.h>

#include <__DRIVER_SLUG__.h>

static int cmd_init(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	const struct device *zdev = DEVICE_DT_GET(DT_ALIAS(__DRIVER_SLUG__));

	if (!device_is_ready(zdev)) {
		shell_error(sh, "device not ready");
		return -ENODEV;
	}

	/* FILL IN: call device functions via __DRIVER_SLUG___from_dev(zdev) */

	shell_print(sh, "ok");
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(__DRIVER_SLUG___cmds,
			       SHELL_CMD(init, NULL, "Placeholder", cmd_init),
			       SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(__DRIVER_SLUG__, &__DRIVER_SLUG___cmds, "__DRIVER_UPPER__", NULL);
#endif

int main(void)
{
	return 0;
}
