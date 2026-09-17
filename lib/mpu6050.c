#include "mpu6050.h"

#include <errno.h>

int mpu6050_init(mpu6050_t *dev, const mpu6050_config_t *cfg)
{
	if (dev == NULL || cfg == NULL) {
		return -EINVAL;
	}

	(void)cfg;

	/* FILL IN: probe the part, apply cfg, cache state on *dev.
	 * Call mpu6050_transfer / mpu6050_delay from
	 * lib/mpu6050_bus.h. Do not add API functions in src/.
	 */

	return 0;
}
