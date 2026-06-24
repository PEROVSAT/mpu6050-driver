#include "mpu6050_lib.h"

#include <errno.h>

/* FILL IN: register addresses and protocol constants */

int mpu6050_lib_init(mpu6050_transfer_fn transfer, void *ctx)
{
	if (transfer == NULL) {
		return -EINVAL;
	}

	/* FILL IN: probe device and apply initial configuration using transfer(ctx, ...) */

	(void)ctx; // Just to avoid warnings, you can delete on implementation

	return 0;
}

/* FILL IN: protocol functions, e.g.:
 *
 * int mpu6050_lib_read_reg(mpu6050_transfer_fn transfer, void *ctx,
 *                                  uint8_t reg, uint8_t *val)
 * {
 *     if (val == NULL) {
 *         return -EINVAL;
 *     }
 *     return transfer(ctx, reg, val, 1, true);
 * }
 */
