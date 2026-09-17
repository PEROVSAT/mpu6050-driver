/*
 * Zephyr packaging: apply DT, bind the bus, call __DRIVER_SLUG___init().
 * Do not add device functions here. See include/__DRIVER_SLUG__.h.
 */

#define DT_DRV_COMPAT __DT_COMPAT__

#include "__DRIVER_SLUG___priv.h"

#if !defined(CONFIG___KCONFIG_SYM___BACKEND_PUBLIC_MOCK)
	#include "__DRIVER_SLUG___bus.h"
#endif

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(__DRIVER_SLUG__, CONFIG_LOG_DEFAULT_LEVEL);

__DRIVER_SLUG___t *__DRIVER_SLUG___from_dev(const struct device *dev)
{
	struct __DRIVER_SLUG___driver_data *data = dev->data;

	return &data->chip;
}

#if !defined(CONFIG___KCONFIG_SYM___BACKEND_PUBLIC_MOCK)
void __DRIVER_SLUG___delay(uint32_t ms)
{
	k_msleep(ms);
}
#endif

static int __DRIVER_SLUG___driver_init(const struct device *dev)
{
	struct __DRIVER_SLUG___driver_data *data = dev->data;
	const struct __DRIVER_SLUG___driver_config *cfg = dev->config;

#if !defined(CONFIG___KCONFIG_SYM___BACKEND_PUBLIC_MOCK)
	int ret;

	data->chip.bus_ctx = (void *)dev;

	ret = __DRIVER_SLUG___transfer_init(dev);
	if (ret < 0) {
		return ret;
	}
#endif

	return __DRIVER_SLUG___init(&data->chip, &cfg->chip);
}

#define __DRIVER_UPPER___INIT(inst)                                                                \
	static struct __DRIVER_SLUG___driver_data __DRIVER_SLUG___data_##inst;                     \
	static const struct __DRIVER_SLUG___driver_config __DRIVER_SLUG___config_##inst = {        \
		.chip =                                                                            \
			{                                                                          \
				.unused = 0,                                                       \
				/* FILL IN: .foo = DT_INST_PROP(inst, foo), */                     \
			},                                                                         \
	};                                                                                         \
	DEVICE_DT_INST_DEFINE(inst, __DRIVER_SLUG___driver_init, NULL,                              \
			      &__DRIVER_SLUG___data_##inst, &__DRIVER_SLUG___config_##inst,         \
			      BOOT_STAGE, BOOT_PRIORITY, NULL);

DT_INST_FOREACH_STATUS_OKAY(__DRIVER_UPPER___INIT)
