#pragma once

/*
 * Zephyr packaging. Do not add device functions here.
 */

#define BOOT_STAGE    POST_KERNEL
#define BOOT_PRIORITY 100

#include <zephyr/device.h>

#include <__DRIVER_SLUG__.h>

struct __DRIVER_SLUG___driver_config {
	__DRIVER_SLUG___config_t chip;

	/* FILL IN when bringing up hardware, e.g. struct i2c_dt_spec bus; */
};

struct __DRIVER_SLUG___driver_data {
	__DRIVER_SLUG___t chip;
};

#if !defined(CONFIG___KCONFIG_SYM___BACKEND_PUBLIC_MOCK)
/* Defined in hardware_transfer.c or lib_mock_transfer.c. */
int __DRIVER_SLUG___transfer_init(const struct device *dev);
#endif
