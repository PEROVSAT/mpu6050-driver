/*
 * Unit tests for the portable device library.
 *
 * Links lib/mpu6050.c only. Do not use src/. When a test talks to
 * the bus, define mpu6050_transfer and mpu6050_delay in
 * this file (see lib/mpu6050_bus.h).
 */

#include <zephyr/ztest.h>

#if 0
	#include "mpu6050.h"
	#include "mpu6050_bus.h"

int mpu6050_transfer(void *ctx, uint8_t reg, uint8_t *buf, size_t len, bool read)
{
	ARG_UNUSED(ctx);
	ARG_UNUSED(reg);
	ARG_UNUSED(buf);
	ARG_UNUSED(len);
	ARG_UNUSED(read);

	return 0;
}

void mpu6050_delay(uint32_t ms)
{
	ARG_UNUSED(ms);
}

ZTEST(mpu6050_unit, test_init)
{
	mpu6050_t dev = {
		.bus_ctx = (void *)1,
	};
	mpu6050_config_t cfg = {0};

	zassert_ok(mpu6050_init(&dev, &cfg));
}
#endif

ZTEST_SUITE(mpu6050_unit, NULL, NULL, NULL, NULL, NULL);
