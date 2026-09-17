#include "__DRIVER_SLUG___bus.h"
#include "__DRIVER_SLUG___priv.h"

#include <errno.h>

int __DRIVER_SLUG___transfer(void *ctx, uint8_t reg, uint8_t *buf, size_t len, bool read)
{
	ARG_UNUSED(ctx);
	ARG_UNUSED(reg);
	ARG_UNUSED(buf);
	ARG_UNUSED(len);
	ARG_UNUSED(read);

	/* FILL IN: talk to the part over I2C, SPI, or UART.
	 *
	 * Also update:
	 *   - dts/bindings (e.g. include: i2c-device.yaml and on-bus: i2c)
	 *   - src/Kconfig  (e.g. select I2C on BACKEND_HARDWARE)
	 *   - priv.h       (bus spec on driver_config)
	 *   - __DRIVER_SLUG__.c DEVICE_DT_INST_DEFINE (e.g. I2C_DT_SPEC_INST_GET)
	 *
	 * I2C example: mpu6050-driver/src/hardware_transfer.c
	 */

	return -ENOTSUP;
}

int __DRIVER_SLUG___transfer_init(const struct device *dev)
{
	ARG_UNUSED(dev);

	/* FILL IN: bus ready check, e.g. i2c_is_ready_dt() */

	return 0;
}
