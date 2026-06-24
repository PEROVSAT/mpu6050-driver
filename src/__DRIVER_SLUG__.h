#pragma once

#define BOOT_STAGE POST_KERNEL
#define BOOT_PRIORITY 100
// Put any other definitions here

#include <zephyr/device.h>

#if !defined(CONFIG___KCONFIG_SYM___BACKEND_PUBLIC_MOCK)
	#include "__DRIVER_SLUG___lib.h"
#endif

// This is the public API definition
struct __DRIVER_SLUG___driver_api {
	// Add function definitions like
	// int (*read_sensor)(const struct device *dev, uint8_t *val);
};


// Static config data, filled at init
struct __DRIVER_SLUG___config {
	// Mirror the devicetree properties here

	#if defined(CONFIG___KCONFIG_SYM___BACKEND_HARDWARE)
		// Add bus spec here
	#endif
};


// Mutable state during runtime
struct __DRIVER_SLUG___data {
	// Things like cached samples go here

	#if defined(CONFIG___KCONFIG_SYM___BACKEND_SIMULATION)
		// Define things like a socket fd
	#endif
};
