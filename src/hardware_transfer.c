#include "mpu6050_bus.h"
#include "mpu6050_priv.h"

#include <errno.h>
#include <string.h>

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(mpu6050);
int mpu6050_transfer(void *ctx, uint8_t reg, uint8_t *buf, size_t len, bool read)
{
	const struct device *dev = ctx;
	const struct mpu6050_driver_config *config = dev->config;
	int ret;

	if (read) {
		ret = i2c_burst_read_dt(&config->bus, reg, buf, len);
	} else {
		uint8_t tx_buf[len + 1];

		tx_buf[0] = reg;
		memcpy(&tx_buf[1], buf, len);

		ret = i2c_write_dt(&config->bus, tx_buf, len + 1);
	}

	if (ret < 0) {
		LOG_ERR("MPU6050 %s: I2C %s failed (reg=0x%02x len=%zu ret=%d)", dev->name,
			read ? "read" : "write", reg, len, ret);
	}

	return ret;
	/* FILL IN: talk to the part over I2C, SPI, or UART.
	 *
	 * Also update:
	 *   - dts/bindings (e.g. include: i2c-device.yaml and on-bus: i2c)
	 *   - src/Kconfig  (e.g. select I2C on BACKEND_HARDWARE)
	 *   - priv.h       (bus spec on driver_config)
	 *   - mpu6050.c DEVICE_DT_INST_DEFINE (e.g. I2C_DT_SPEC_INST_GET)
	 *
	 * I2C example: mpu6050-driver/src/hardware_transfer.c
	 */
}

int mpu6050_transfer_init(const struct device *dev)
{
	const struct mpu6050_driver_config *config = dev->config;

	LOG_DBG("MPU6050 %s: checking I2C bus %s addr 0x%02x", dev->name, config->bus.bus->name,
		config->bus.addr);

	if (!i2c_is_ready_dt(&config->bus)) {
		LOG_ERR("MPU6050 %s: I2C bus not ready (bus=%s addr=0x%02x)", dev->name,
			config->bus.bus->name, config->bus.addr);
		return -ENODEV;
	}

	LOG_DBG("MPU6050 %s: I2C bus ready", dev->name);

	return 0;
}
