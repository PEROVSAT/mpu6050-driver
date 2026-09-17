#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Device API. Adding a function:
 *   1. Declare it here
 *   2. Implement it in lib/__DRIVER_SLUG__.c
 *   3. Stub it in mock/__DRIVER_SLUG__.c
 *
 * Do not edit src/ for API work.
 */

typedef struct {
	/* FILL IN: DT-backed settings used by init. Remove if unused. */
	uint8_t unused;
} __DRIVER_SLUG___config_t;

typedef struct __DRIVER_SLUG__ {
	/*
	 * Which instance to talk to. src/ sets this to the Zephyr device
	 * pointer; unit tests set it to any non-NULL token. App code should
	 * not read or write it.
	 *
	 * The bus implementation is a named function (__DRIVER_SLUG___transfer),
	 * not a pointer on this object. See lib/__DRIVER_SLUG___bus.h.
	 */
	void *bus_ctx;

	/* FILL IN: cached chip state used after init. */
} __DRIVER_SLUG___t;

int __DRIVER_SLUG___init(__DRIVER_SLUG___t *dev, const __DRIVER_SLUG___config_t *cfg);

/* App glue: Zephyr `struct device` → this object. Implemented in src/. */
struct device;
__DRIVER_SLUG___t *__DRIVER_SLUG___from_dev(const struct device *dev);

#ifdef __cplusplus
}
#endif
