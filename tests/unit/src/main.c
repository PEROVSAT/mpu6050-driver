/*
 * Unit tests for the portable device library.
 *
 * Links lib/__DRIVER_SLUG__.c only. Do not use src/. When a test talks to
 * the bus, define __DRIVER_SLUG___transfer and __DRIVER_SLUG___delay in
 * this file (see lib/__DRIVER_SLUG___bus.h).
 */

#include <zephyr/ztest.h>

#if 0
#include "__DRIVER_SLUG__.h"
#include "__DRIVER_SLUG___bus.h"

int __DRIVER_SLUG___transfer(void *ctx, uint8_t reg, uint8_t *buf, size_t len, bool read)
{
	ARG_UNUSED(ctx);
	ARG_UNUSED(reg);
	ARG_UNUSED(buf);
	ARG_UNUSED(len);
	ARG_UNUSED(read);

	return 0;
}

void __DRIVER_SLUG___delay(uint32_t ms)
{
	ARG_UNUSED(ms);
}

ZTEST(__DRIVER_SLUG___unit, test_init)
{
	__DRIVER_SLUG___t dev = {
		.bus_ctx = (void *)1,
	};
	__DRIVER_SLUG___config_t cfg = {0};

	zassert_ok(__DRIVER_SLUG___init(&dev, &cfg));
}
#endif

ZTEST_SUITE(__DRIVER_SLUG___unit, NULL, NULL, NULL, NULL, NULL);
