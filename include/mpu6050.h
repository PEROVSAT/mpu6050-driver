#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Device API. Adding a function:
 *   1. Declare it here
 *   2. Implement it in lib/mpu6050.c
 *   3. Stub it in mock/mpu6050.c
 *
 * Do not edit src/ for API work.
 */

typedef struct {
	/* FILL IN: DT-backed settings used by init. Remove if unused. */
	uint8_t unused;
} mpu6050_config_t;

typedef struct mpu6050 {
	/*
	 * Which instance to talk to. src/ sets this to the Zephyr device
	 * pointer; unit tests set it to any non-NULL token. App code should
	 * not read or write it.
	 *
	 * The bus implementation is a named function (mpu6050_transfer),
	 * not a pointer on this object. See lib/mpu6050_bus.h.
	 */
	void *bus_ctx;

	/* FILL IN: cached chip state used after init. */
} mpu6050_t;

int mpu6050_init(mpu6050_t *dev, const mpu6050_config_t *cfg);

/* App glue: Zephyr `struct device` → this object. Implemented in src/. */
struct device;
mpu6050_t *mpu6050_from_dev(const struct device *dev);

#ifdef __cplusplus
}
#endif
