#define DT_DRV_COMPAT __DT_COMPAT__

#include "__DRIVER_SLUG__.h"

#include <zephyr/device.h>
#include <zephyr/logging/log.h>

#if !defined(CONFIG___KCONFIG_SYM___BACKEND_PUBLIC_MOCK)
	#include "transfer.h"
#endif

LOG_MODULE_REGISTER(__DRIVER_SLUG__, CONFIG_LOG_DEFAULT_LEVEL);

// Static API functions that are defined in __DRIVER_SLUG__.h get implemented here

/*
static int read_sensor(const struct device *dev, uint8_t *val)
{
	#if !defined(CONFIG___KCONFIG_SYM___BACKEND_PUBLIC_MOCK)
		return __DRIVER_SLUG___lib_read_sensor(__DRIVER_SLUG___transfer, dev, val);
	#else
		ARG_UNUSED(dev);
		*val = 0x01;
		return 0;
	#endif
}
*/


const struct __DRIVER_SLUG___driver_api __DRIVER_SLUG___api = {
	// .read_sensor = read_sensor,
};


static int __DRIVER_SLUG___init(const struct device *dev)
{
	#if !defined(CONFIG___KCONFIG_SYM___BACKEND_PUBLIC_MOCK)
		// Initialize transfer backend and library
		int ret = __DRIVER_SLUG___transfer_init(dev);
		if (ret < 0) {
			return ret;
		}

		return __DRIVER_SLUG___lib_init(__DRIVER_SLUG___transfer, dev);
	#else
		ARG_UNUSED(dev);
		return 0;
	#endif
}


#define __DRIVER_UPPER___INIT(inst)                                                                \
	static struct __DRIVER_SLUG___data __DRIVER_SLUG___data_##inst;                            \
	static const struct __DRIVER_SLUG___config __DRIVER_SLUG___config_##inst = {               \
		/* FILL IN: .bus = I2C_DT_SPEC_INST_GET(inst), */                                     \
	};                                                                                         \
	DEVICE_DT_INST_DEFINE(inst, __DRIVER_SLUG___init, NULL, &__DRIVER_SLUG___data_##inst,      \
			      &__DRIVER_SLUG___config_##inst, BOOT_STAGE, BOOT_PRIORITY,             \
			      &__DRIVER_SLUG___api);

DT_INST_FOREACH_STATUS_OKAY(__DRIVER_UPPER___INIT)
